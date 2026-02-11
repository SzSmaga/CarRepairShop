

#ifndef FRONTDESK_H
#define FRONTDESK_H

// Logika Allow-List
int is_brand_allowed(CarBrand brand) {
    switch (brand) {
    case 'A': case 'E': case 'I': case 'O': case 'U': case 'Y': return 1;
    default: return 0;
    }
}




#endif //FRONTDESK_H
