#include <stdio.h>
#include <stdlib.h>

struct lala
{
	int a;
	int b;
};

int func2(struct lala **p2)
{
	struct lala *p3;

	p3 = (struct lala *)malloc(sizeof(struct lala));

	if(p3 == NULL){
		printf("p3 is null\n");
	}

	p3->a = 100;

	*p2 = p3;

	return 0;
}

int func1(struct lala **p1)
{
	if(p1 == NULL){
		printf("A pet is %s\n", p1 ? "not null" : "null");
	}
	
	func2(p1);

	return 0;	
}

int main(int argc, char **argv)
{
	struct lala *pet = NULL;

	func1(&pet);

	printf("B pet is %s\n", pet ? "not null" : "null");
	if(pet){
		printf("pet->a is %d\n",pet->a);
	}
}
