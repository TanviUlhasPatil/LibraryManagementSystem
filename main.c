#include <mysql.h>
#include <stdio.h>

struct books{
    int id;
    char bookName[50];
    char authorName[50];
    int pages;
};

struct student{
    int id;
    char sName[50];
    char sClass[10];
    int sRoll;
};
MYSQL *conn;
MYSQL_RES *res;
MYSQL_ROW row;

struct books AddBook(struct books);
int bookList();
int DeleteBook();
struct student IssueBook(struct student);
int IssueBookList();

int main()
{
	struct books b;
	struct student s;
	MYSQL_RES *res;
 	MYSQL_ROW row;

 char *server ="localhost";
 char *user = "root";
 char *password = "123456"; 
 char *database = "mydb";

 conn = mysql_init(NULL);

 if (!mysql_real_connect(conn, server,user, password, database, 0, NULL, 0)) //Connect to database
 {
   printf("Failed to connect MySQL Server %s. Error: %s\n", server, mysql_error(conn));
   return 0;
 }

 if (mysql_query(conn, "show tables"))  //Execute SQL query to fetch all table names.*/
 {
   printf("Failed to execute quesry. Error: %s\n", mysql_error(conn));
   return 0;
 }

 res = mysql_use_result(conn);

 mysql_free_result(res);
	int choice=0;
	printf(" <== Library Management System ==>\n\n");
	
	while(1)
	{
		printf("\n\t\t1.Add Book");
		printf("\n\t\t2.View Book");
		printf("\n\t\t3.Remove Book");
		printf("\n\t\t4.Issue Book");
		printf("\n\t\t5.Issued Book List");
		printf("\n\t\t6.Exit");
		
		printf("\nEnter Your Choice:");
		scanf("%d",&choice);
		printf("\n");
		switch(choice)
		{
			case 1:
				b=AddBook(b);
				break;
			
			case 2:
				
				bookList();
				break;	
				
			case 3:
				DeleteBook();
				break;
			
			case 4:
				IssueBook(s);
				break;
					
			case 5:
				IssueBookList();
				break;						
			case 6:
				exit(6);
            break;

        	default:
            	printf("\nInvalid Choice...\n\n");		
		}
	}
	
 return 0;
}

struct books AddBook(struct books b){
	char buf[1024]={};
	int f=0;
    printf("Enter Book Id: ");
    scanf("%d",&b.id);

    printf("Enter Book Name: ");
    fflush(stdin);
    gets(b.bookName);

	printf("Enter Author Name: ");
    fflush(stdin);
    gets(b.authorName);
    
    printf("Enter Total Pages: ");
    scanf("%d",&b.pages);

	char query[]={"INSERT INTO books(id, bname, authorname, pages) VALUES( %d,'%s','%s',%d)"};
	sprintf(buf,query,b.id,b.bookName,b.authorName,b.pages);
	if(mysql_query(conn,buf))
 	{
 		printf("Failed to execute quesry. Error: %s\n", mysql_error(conn));
 		printf("%d",f);
 	}
 	else
	{
 		printf("\nBook Added Successfully\n");
	}
    return b;
}

int bookList(){
	printf("\t\t\t<== Available Books ==>\n\n");
	printf("%-20s %-20s %-20s %-10s\n\n", "Book id", "Book Name", "Author Name", "Total Pages");
	
   	if (mysql_query(conn, "select * from books"))
 	{
   		printf("Failed to execute quesry. Error: %s\n", mysql_error(conn));	
		mysql_close(conn);	
    	return 0;
 	}
 	res = mysql_store_result(conn);
 	if (res == NULL)
 	{
 		printf("\nUnable to Compile SQL statement");
 		mysql_close(conn);
    	return 1;
 	}
 	while(row = mysql_fetch_row(res))
 		printf("%-20s %-20s %-20s %-10s\n",row[0],row[1],row[2],row[3]);
 		
  	mysql_free_result(res);
    return 1;
}

int DeleteBook()
{
	char query[1024]={};
	int id,flag=0;

    printf("Enter Book id to Remove: ");
    scanf("%d", &id);

	sprintf(query,"DELETE FROM books where id=%d",id);
	if(mysql_query(conn,query))
 	{
 		printf("Failed to execute quesry. Error: %s\n", mysql_error(conn));
 		mysql_close(conn);
 		return 0;
 	}
 	else
	{
 		printf("\nBook Removed Successfully\n");
	}
    return 1;
}

struct student IssueBook(struct student s)
{
	char buf1[1024]={};
	printf("\t\t\t<== Issue Books ==>\n\n");

    printf("Enter Book id to issue: ");
    scanf("%d",&s.id);
    
    printf("Enter Student Name: ");
    fflush(stdin);
    gets(s.sName);

    printf("Enter Student Class: ");
    fflush(stdin);
    gets(s.sClass);

    printf("Enter Student Roll: ");
    scanf("%d",&s.sRoll);
    
    char query1[]={"INSERT INTO data(bookid, sname, sclass, sroll) VALUES(%d,'%s','%s',%d)"};
	sprintf(buf1,query1,s.id,s.sName,s.sClass,s.sRoll);
	
    if (mysql_query(conn, buf1))
 	{
   		printf("Failed to execute quesry. Error: %s\n", mysql_error(conn));	
 	}
 	printf("\nBook Issued Successfully\n");
    return s; 
}

int IssueBookList(){
	printf("\t\t\t<== Issued Books List==>\n\n");
	printf("%-20s %-20s %-20s %-20s %-20s %-10s\n\n", "Issued Book id", "Student Name", "Student Class", "Student Roll No", "Book Name", "Author Name");
	
   	if (mysql_query(conn, "SELECT bookid,sname,sclass,sroll,bname,authorname FROM books b JOIN data d ON b.id=d.bookid;"))
 	{
   		printf("Failed to execute quesry. Error: %s\n", mysql_error(conn));	
		mysql_close(conn);	
    	return 0;
 	}
 	res = mysql_store_result(conn);
 	if (res == NULL)
 	{
 		printf("\nUnable to Compile SQL statement");
 		mysql_close(conn);
    	return 1;
 	}
 	while(row = mysql_fetch_row(res))
 		printf("%-20s %-20s %-20s %-20s %-20s %-10s\n",row[0],row[1],row[2],row[3],row[4],row[5]);
 		
  	mysql_free_result(res);
    return 1;
}
