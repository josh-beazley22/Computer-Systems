
typedef struct {
    unsigned char valid: 1;
    unsigned char physical_page_no: 4;
} PageTableEntry;

#define PAGE_TABLE_SIZE     16

typedef struct {
    PageTableEntry entries[PAGE_TABLE_SIZE];
} PageTable;

// Caching system called the TLB
typedef struct {
    unsigned int valid: 1;
    unsigned short order: 2;
    unsigned short virtual_page_no: 4;
    unsigned short physical_page_no: 4;
} TLB_Entry;

#define TLB_SIZE    4

typedef struct {
    TLB_Entry entries[TLB_SIZE];
} TLB;