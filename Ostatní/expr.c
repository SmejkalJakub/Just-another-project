#include "expr.h"


#define EQ 0
#define L 1
#define B 2
#define ER 3


#define TSIZE 7



#define EXPR_PLUS 0
#define EXPR_
/*E -> E + E
E – E
E < E
E > E
E * E
E / E
E // E
E <= E
E >= E
E != E
E == E
E
id
*/

/*	
"+/-"	"*\/"	r	i	"("	")"	$
"+/-"	>	<	>	<	<	>	>
"*\/"	>	>	>	<	<	>	>
r	<	<		<	<	>	>
i	>	>	>			>	>
"("	<	<	<	<	<	"="	
")"	>	>	>			>	>
$	<	<	<	<	<		
*/


int precedenceTable[TSIZE][TSIZE] = 
{
    {B, L, B, L, L, B, B},
    {B, B, B, L, L, B, B},
    {L, L, ER, L, L, B, B},
    {B, B, B, ER, ER, B, B},
    {L, L, L, L, L, EQ, ER},
    {B, B, B, ER, ER, B, B},
    {L, L, L, L, L, ER, ER},
};
