#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
	freopen("data.txt","w",stdout);
	srand(time(0));
	const int lines=5000;
	const int lenth_per_line=20;
	for (int i=0; i<lines; i++) {
		for (int j=0;j<lenth_per_line;j++) {
			printf("%c",(char)(rand()%26+'a'));
		}	
		printf("\n");
	}
}
