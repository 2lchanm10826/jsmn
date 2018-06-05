
typedef struct {
  int tokindex;
  int objectindex;
} NameTokenInfo;


typedef enum{
   ORION  = 0,
   LOTTE = 1,
   NONGSIM = 2,
   CROWN = 3
} company_t;


typedef struct {
  company_t company;
  char name[20];
  int price;
  int count;
} product_t ;


product_t * snacklist[20];
