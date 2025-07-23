#include <stdio.h>


typedef struct {

     char *data;

     int key;

} item;

 

item array[] = {

     {"Bill", 62},

     {"Hill", 60},

     {"Barcak", 42},

     {"Dicky", 105},

     {"W.", 1},

};

  

// Sort the items

void sort(item *a, int n) { 

      int i = 0, j = 0;

      int s;


      for (; i < n && s != 0; i++) {

              s = 0;

              for (j = 0; j < n; j++) {

                       if (a[j].key > a[j+1].key) {

                               item t = a[j];

                               a[j] = a[j+1];

                               a[j+1] = t;

                               s++;

                       }

               }

              n--;

       }

}

   

int main() {

    sort(array,5); 

    return 0;

}
