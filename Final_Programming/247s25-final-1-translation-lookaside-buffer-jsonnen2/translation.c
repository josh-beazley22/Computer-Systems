#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include "translation.h"


// Given a virtual address, return the virtual page no.
// The virtual page number is the upper 4 bits of the 16 bit virtual
// address.
unsigned short to_page(unsigned short virtual_address) {
    return (virtual_address & 0xf000) >> 12;
}


// Given a virtual address, return the offset within the physical page.
// This is the lower 12 bits of the address.
unsigned short to_offset(unsigned short virtual_address) {
    return (virtual_address & 0x0fff);
}


// Create an address out of a page number and an offset
unsigned short to_address(unsigned short page_no, unsigned short offset) {
    return (page_no << 12) | (offset & 0x0fff);
}


// Dump the TLB to stdout
void tlb_dump(TLB* tlb) {
    for (int i = 0; i < TLB_SIZE; i++) {
        TLB_Entry* entry = &tlb->entries[i];
        printf("%d: Valid:%d, Virtual:%x,Physical:%x, Order:%d\n", i,
            entry->valid, entry->virtual_page_no, entry->physical_page_no,
            entry->order);
    }
}


// Evict an entry from the TLB and read in the corresponding page table
// entry for the given virtual page number from the page table. The replacement
// policy is LRU. Return a pointer to the the new TLB entry
TLB_Entry* tlb_load(TLB* tlb,
                    unsigned short virtual_page_no,
                    PageTable* ptab) {
    unsigned short my_physical = ptab->entries[virtual_page_no].physical_page_no;
    for (int i = 0; i < TLB_SIZE; i++) {
        if (tlb->entries[i].order == 3) {
            // last order elem in TLB
            tlb_set_order(tlb, i);
            // create new entry
            TLB_Entry* entry;
            tlb->entries[i].valid = 1;
            tlb->entries[i].order = 0;
            tlb->entries[i].virtual_page_no = virtual_page_no;
            tlb->entries[i].physical_page_no = my_physical;
            return &tlb->entries[i];
        }
    }
}

// Update the last used order for the TLB Entry
void tlb_set_order(TLB* tlb, int last_used_index) {
    unsigned char comp = tlb->entries[last_used_index].order;
    for (int i = 0; i < TLB_SIZE; i++) {
        if (tlb->entries[i].order < comp) {
            tlb->entries[i].order += 1;
        }
    }
    tlb->entries[last_used_index].order = 0;
}


// Lookup a virtual address. Given a virtual address, lookup the virtual
// address in the TLB. If the virtual address is present in the TLB,
// set the physical address in the given pointer argument and return true.
// If the TLB does not contain the virtual address, return false.
// The lookup order in the TLB is updated.
bool tlb_lookup(TLB* tlb, unsigned short virtual_address,
                unsigned short* physical_address) {
    unsigned short my_page_no = to_page(virtual_address);
    unsigned short my_offset = to_offset(virtual_address);
    for (int i = 0; i < TLB_SIZE; i++) {
        if (tlb->entries[i].virtual_page_no == my_page_no) {
            *physical_address = to_address(tlb->entries[i].physical_page_no, my_offset);
            tlb_set_order(tlb, i);
            return true;
        }
    }
    return false;
}


// Lookup a virtual address. Given a virtual address, lookup the virtual
// address in the TLB to get the physical address. If the TLB doesn't hold the
// physical address, load it from the page table. If the page table entry
// is invalid, return false. If the page table is valid, set
// *physical_address to the physical address, and return true.
bool tlb_lookup_and_load(TLB* tlb, PageTable* ptab,
                         unsigned short virtual_address,
                         unsigned short* physical_address) {
    if (! tlb_lookup(tlb, virtual_address, physical_address)) {
        unsigned short virtual_page_no = to_page(virtual_address);
        TLB_Entry* tlb_entry = tlb_load(tlb, virtual_page_no, ptab);
        if (tlb_entry == NULL) {
            return false;
        }
        if (tlb_entry->valid) {
            *physical_address = to_address(tlb_entry->physical_page_no,
                                  to_offset(virtual_address));
            return true;
        }
    }
    return false;
}
