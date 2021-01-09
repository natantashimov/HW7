#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include  "grades.h"
#include  "linked-list.h"

struct list *find_iterator(struct grades *grades, int id);
bool check_id(struct grades *grades, int id);

struct course{
    char *course_name;
    int course_grade;
    struct course *next_course;
    };

struct grades{
	char *student_name;
	int id;
    struct list *courses;
    struct grades *next;
};
	

//user function	
int clone_course(void *elem, void **output){
	struct course *course;
	struct course **out;
	course = (struct course*)elem;
	out = (struct course**)output;

	*out = (struct course*) malloc(sizeof(course));
	if(!(*out)){
		return -1;
	}
	(*out)->course_name = (char*)malloc(sizeof(char)*(strlen(course->course_name)+1));
	if(!(*out)->course_name){
		return -1;
	}
	strcpy((*out)->course_name, course->course_name);

	(*out)->course_grade = course->course_grade;
	
	return 0;
}
	
//user function
void destroy_course(void *elem){
	if(!elem){
		return;
	}
	struct course *course;
	course = (struct course*)elem;
	free(course->course_name);
	free(course);
}



struct grades* grades_init(){
    struct grades *ptr =malloc(sizeof(struct grades));
    if(!ptr){
    	return NULL;
    }
    struct list *course_list= list_init(clone_course,destroy_course);
    ptr->courses = course_list;
  	return ptr;	
}

void grades_destroy(struct grades *grades){
    struct grades *tmp;
    while (grades != NULL)
    {
       tmp = grades;
       grades = grades->next;
       free(tmp->courses);
       free(tmp);
    }
}


//return true if id already exist in grades
bool check_id(struct grades *grades, int id){
	struct grades *current_student;
	current_student = grades;
	while(current_student){
		if(current_student->id == id){
			return true;
		}
		current_student = current_student->next;
	}
}


struct list *find_iterator(struct grades *grades, int id){
    struct grades *current = grades;
    while (current != NULL){
		if (current->id == id){
		     struct list *student_courses= current->courses;
		     return student_courses;
		}
		current = current->next;
	}
}


int grades_add_student(struct grades *grades ,const char *name, int id){
	if(!grades){
		return -1;
	}
	if(check_id(grades, id)){
		return -1;
	}else{   
		struct grades *temp = grades;             // need to add the student
		while(temp->next){
			temp = temp->next;
		}
		struct grades *new_student = grades_init(); 
		new_student->id = id;
		strcpy(new_student->student_name , name);
		new_student->next = NULL;
		temp->next = new_student;
		return 0;
	}

}
int grades_add_grade(struct grades *grades,
                     const char *name,
                     int id,
                     int grade){
	if(!grades){
		return -1;
	}
	if(grade>100 || grade<0){
		return -1;
	}
	if(!check_id(grades, id))
	{
		return -1;
	}else{
		struct list *student_courses = find_iterator(grades, id);
		struct course *new_course = malloc(sizeof(struct course));
		strcpy(new_course->course_name , name);
		new_course->course_grade = grade;
		int push = list_push_back (student_courses, new_course);
		return 0;
	}

/*		struct grades *current_student;
		current_student = grades;
		while(current_student){
			if(current_student->id == id){
				struct list *student_courses = current_student->courses;
				struct list *new_course = malloc(sizeof(struct course));
				new_course->course_name = name;
				new_course->course_grade = grade;
				int push = list_push_back (student_courses, new_course);
				return 0;
			}
			current_student = current_student->next;
		}
	}*/
}


float grades_calc_avg(struct grades *grades, int id, char **out){
	int sum = 0;
	int temp=1;
	if(!grades){
		return -1;
	}
	if(!check_id(grades, id)){
		return -1;
	}else{
		struct grades *current_student;
		current_student = grades;
		while(current_student){
			if(current_student->id == id){
				struct list *student_courses = current_student->courses;
				temp = list_size(student_courses);		
				struct course *course;
				course = (struct course*)(list_begin(student_courses));
				sum = sum + (course->course_grade);
				while(course){
					course =(struct course*)(list_next(course));
					if(course->course_grade){
						sum = sum + course->course_grade;	
					}			
				}
			}
			current_student = current_student->next;
		}
		
	}
	return (float)(sum/temp);

}



int grades_print_student(struct grades *grades, int id){
	if(!grades){
		return -1;
	}
	if(!check_id(grades, id)){
		return -1;
	}else{
		struct grades *current_student;
		current_student = grades;
		while(current_student){
			if(current_student->id == id){
				struct list *student_courses = current_student->courses;
				printf("student name:" " %s",current_student->student_name);
     			printf("student id: " " %d",id);
     			struct course *course;
     			course = (struct course*)(list_begin(student_courses));
     			for(int i = 1 ; i <= list_size(student_courses);i++){
     				printf("course ""%d ""name is: " "%s",i, course->course_name);
     				printf("course ""%d ""grade is: " "%d",i,course->course_grade);
     			}
     			return 0;
			}
			current_student = current_student->next;
		}
	}
	return -1;
}



int grades_print_all(struct grades *grades){
	struct grades *current_student;
	current_student = grades;
	if(!grades){
		return -1;
	}
	int id = 0;
	while(current_student){
		id = current_student->id;
		int print = grades_print_student(grades, id);
		current_student = current_student->next;
	}
	return 0;
}