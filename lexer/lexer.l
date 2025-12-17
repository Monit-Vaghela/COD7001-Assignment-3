%{
#include "parser.tab.h"
%}

%%
/* token rules go here */
%%

int yywrap(void) {
    return 1;
}
