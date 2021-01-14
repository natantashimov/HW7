#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "linked-list.h"
#include "grades.h"
#define FAIL -1
struct node *node;
struct course{
	const char *course_name;
	int grade;
};
struct student{
    int id;
    char *name;
    struct list *courses;
};
struct grades{
    struct list *students;
};
/* 
declaration of helper functions 
*/
static bool id_exist(struct grades *grades,int id);
static int clone_course(void *element, void **output);
static int clone_student(void *element, void **output);
static void destroy_course(void *element);
static void destroy_student(void *element);

/* 
declaration of functions that appear in the grades.h
*/
struct grades* grades_init();
void grades_destroy(struct grades *grades);
int grades_add_student(struct grades *grades, const char *name, int id);
int grades_add_grade(struct grades *grades,const char *name,int id,int grade);
float grades_calc_avg(struct grades *grades, int id, char **out);
int grades_print_student(struct grades *grades, int id);
int grades_print_all(struct grades *grades);

/*
this function returns true if the student id already exists and false otherwise 
*/
static bool id_exist(struct grades *grades,int id){
    if(!grades){
        return false;
    }
    struct node *iterator = list_begin(grades->students);
    struct student *current_student;
    while(iterator){
        current_student = list_get(iterator);
        if(current_student->id == id){
            return true;
        }
        iterator = list_next(iterator);                                                                                                                                                                        
    }
    return false;
}
/*
 the 4 functions below are user functions for the linked-list adt
*/
static int clone_course(void *element, void **output){
    struct course *temp = (struct course*)element;
    struct course *add_course;
    add_course = (struct course*)malloc(sizeof(struct course));
    if(!add_course){
        printf("Failed to allocate");
        return FAIL;
    }
    add_course->grade = temp->grade;
    add_course->course_name = (char*)malloc(sizeof(char)*strlen
    	(temp->course_name)+1);
    if(!add_course->course_name){
        printf("Failed to allocate");
        free(add_course);
        return FAIL;
    }
    strcpy(add_course->course_name,temp->course_name);
    *output = (void*)add_course;
    return 0;
}
static int clone_student(void *element, void **output) {
    struct student *temp = (struct student*)element;
    struct student *add_student;
    add_student = (struct student*)malloc(sizeof(struct student));
    if(!add_student){
        printf("Failed to allocate");
        return FAIL;
    }
    add_student->id = temp->id;
    add_student->name = (char*)malloc(sizeof(char)*(strlen(temp -> name)+1));
    if(!(add_student->name)){
        printf("Failed to allocate");
        free(add_student);
        return -1;
    }
    strcpy(add_student->name, temp->name);
    add_student->courses = list_init(clone_course,destroy_course);
    *output = (struct student*)add_student;
    return 0;
}
static void destroy_course(void *element){
    struct course *course_to_destroy = (struct course*)element;
    free(course_to_destroy->course_name);
    free(course_to_destroy);
}
static void destroy_student(void *element){
    struct student *student_to_destroy = (struct student*)element;   
    free(student_to_destroy->name);
    list_destroy(student_to_destroy->courses);
    free(student_to_destroy);
}

/**
 * this function initialize the data structure.
 * returns a pointer to the data-structure, or NULL in case of an error
 */
struct grades *grades_init(){
    struct grades *grades;
    grades = (struct grades*)malloc(sizeof(struct grades));
    if (!grades) {
        return NULL;
    }
    grades->students = list_init (clone_student,destroy_student);
    return grades;
}

/**
 *this function Destroy grades, and de-allocate all memory
 */
void grades_destroy(struct grades *grades){
    list_destroy(grades->students);
    free(grades);
}
/**
 * adds a student with name and id to grades
 *  if grades is invalid, or a student with 
 * the same id already exists in grades it will fail.
 */
int grades_add_student(struct grades *grades, const char *name, int id){
    if(id_exist(grades,id)){ 
        return FAIL;
    }
    struct student add_student;
    add_student.id = id;
    add_student.name = (char*)malloc(sizeof(char)*strlen(name)+1);
    if(!add_student.name){
        printf("Failed to allocate");
        return FAIL;
    }
    strcpy(add_student.name,name);
    add_student.courses = list_init(clone_course,destroy_course);
    if(!add_student.courses){
        free(add_student.name);
        printf("Failed to initialize the courses list");
        return FAIL;
    }
    struct student *temp = (void*)(&add_student);
    list_push_back(grades->students,temp);
    free(add_student.name);
    list_destroy(add_student.courses);
    return 0;
}



/**
 * adds a course with "name" and "grade" to the student with "id"
 */
int grades_add_grade(struct grades *grades,const char *name,int id,int grade){
    if(!(id_exist(grades,id)) || !(grade>=0 && grade<=100)){ 
        return FAIL;
    }
    struct node *student_iterator = list_begin(grades->students);
    struct student *student;
    while(student_iterator != NULL){
        student = (struct student*)list_get(student_iterator);
        if(!student){
            return FAIL;
        }
        if(student->id == id){
            break;
        }
        student_iterator = list_next(student_iterator);
    }
    struct node *course_iterator = list_begin(student->courses);
    struct course *temp_course; 
    while (course_iterator){
        temp_course = (struct course*)list_get(course_iterator);
        if(!strcmp(temp_course->course_name,name)){
            return FAIL;
        }
        course_iterator = list_next(course_iterator);
    }
    struct course new_course;
    new_course.grade = grade;
    new_course.course_name = malloc(sizeof(char)*strlen(name)+1);
    if(!new_course.course_name){
        printf("Failed to allocate");
        return FAIL;
    }
    strcpy(new_course.course_name,name);
    struct course *pointer_to_new_course = (void*)(&new_course);
    if(!list_push_back(student->courses,pointer_to_new_course)){
        free(new_course.course_name);
        return 0; 
    } else{
        free(new_course.course_name);
        printf("Failed to add grade");
        return FAIL;
    }
}


/**
 * this function calculating the average of the student with "id" in "grades".
 * returns The average, or -1 if error
 */
float grades_calc_avg(struct grades *grades , int id,char **out){
    if(!id_exist(grades,id)){
        *out = NULL;
        return FAIL;
    }
  	float avg = 0,sum = 0;
  	struct node *iterator = list_begin(grades->students);
  	struct student *student_to_calc;
  	while(iterator){
    	student_to_calc = (struct student*)list_get(iterator);
    	if(student_to_calc->id == id){
        	char *name_of_student = (char*) malloc(strlen(student_to_calc->name)+1);
        	if(name_of_student == NULL){
            	printf("Failed to Allocate Bytes");
            	*out=NULL;
            	return FAIL;
        	}
        	strcpy(name_of_student ,student_to_calc->name);
        	*out = name_of_student;
        	break;
        }
        iterator = list_next(iterator);
    }
    struct node *courses_iterator = list_begin(student_to_calc->courses);
    while(courses_iterator){
        struct course *course = (struct course*)list_get(courses_iterator);
        sum = sum + course->grade;
        courses_iterator = list_next(courses_iterator);
    }
  	if(!(list_size(student_to_calc->courses))){
    	avg = 0;
  	} else {
    	avg = (float)(sum/list_size(student_to_calc->courses));
  	}
  	return avg;
}
/**
 * this function print details about student with id in grade.
 */
int grades_print_student(struct grades *grades, int id){
    if(!id_exist(grades,id)){
        return FAIL;
    }
    struct student *student;
    struct node *student_iterator;
    struct node *course_iterator;
    student_iterator = list_begin(grades->students); 
    while (student_iterator){
        student = (struct student*)list_get(student_iterator);
        if(student->id == id){
            course_iterator = list_begin(student->courses);
            printf("%s %d: ",student->name,student->id);
            if(course_iterator){
                while(course_iterator){
                    struct course *course = (struct course*)list_get
                    (course_iterator);
                    printf("%s %d",course->course_name,course->grade);
                    course_iterator = list_next(course_iterator);
                    if(course_iterator){
                        printf(", ");
                    }
                }
            }
            printf("\n");
            return 0;
        }
        student_iterator = list_next(student_iterator);
    }
    return FAIL;
}
/**
 * this function prints all of the students in grade 
 * and setting pointer iterator by pointing to the first student
 */
int grades_print_all(struct grades *grades){
    if(!grades){
        return FAIL;
    }
    struct student *student;
    struct node *iterator = list_begin(grades->students);
    while(iterator){
        student = (struct student*)list_get(iterator);
        if(grades_print_student(grades,student->id)){
            return FAIL;
        }
        iterator = list_next(iterator);
    }
    return 0;
}