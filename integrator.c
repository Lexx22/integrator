/*
 * Без имени.c
 * 
 * Copyright 2014 Lex <lex22@ubuntu>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */


#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>

double f(double x){
	return x;
}
//dx < delta!!!!!!!!!!!!!!!!!

double integr(double a, double b, double dx, double(*f)(double)){
	double result = 0;
	double x = a;
	for(; x < b; x += dx){
		result += (f(x + dx) + f(x)) / 2 * dx;
	}
	//printf("%lf\n", result);
	return result;
}

double forking(double a, double b, double dx, double(*f)(double), int pr_num){
	double delta = (b - a) / pr_num;
	int fd[2];
	pipe(fd);
	int i = 0;
	for(; i < pr_num; ++i){
		int pid = fork();//pid - process identificator
		if(pid == 0){//kind
			close(fd[0]);
			double res = integr(a + delta * i, a + delta * (i + 1), dx, f);
			write(fd[1], &res, sizeof(res));
			close(fd[1]);
			exit(0);
		}
		else{//parent
			if(pid == -1){
				printf("error /n");
			}
		}
	}
	//parents actions
	close(fd[1]);
	double res, res_total = 0;
	int j = 0;
	for(; j < pr_num; ++j){
		read(fd[0], &res, sizeof(res));//return 0 if dont read
		res_total += res;	
	}
	close(fd[0]);
	return res_total;
}

int main (int argc, char *argv[]){
	double res = forking(0, 6, 1, f, 6);
	printf("%lf\n", res);
	return 0;
}
