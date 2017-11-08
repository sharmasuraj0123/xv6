#include "types.h"
#include "stat.h"
#include "user.h"


int
main(void)
{

 void *buffer = malloc(2997);
 wolfie(buffer,2997);

 printf(1,"%s\n",(char *)buffer);

 return 0;
}

