//Libraries used: The project uses standard C libraries such as stdio.h, string.h, and stdlib.h.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

//Global constant for easy file handling
#define FILE_NAME "EmployeeData.txt"

// Define the structure for employee data
struct EmployeeData{
    int id;
    char name[100];
    int age;
    char department[100];
    char jobPosition[100];
    int daysRendered;
    float salary;
}Employee;

//Function to get daily rate based on job position
float getStandardDailyRate(const char *jobPosition){
    if(strcmp(jobPosition, "Manager") == 0 || strcmp(jobPosition, "manager") == 0){
        return 1500.0;
    }
    else if(strcmp(jobPosition, "Staff") == 0 || strcmp(jobPosition, "staff") == 0){
        return 1000.0;
    }
    else if(strcmp(jobPosition, "Clerk") == 0 || strcmp(jobPosition, "clerk") == 0){
        return 800.0;
    } 
    else{
        return 500.0;
    }
}

//Edge Handling Functions prototypes
void checkValidString(const char *prompt, char *errorMessage, int maxLength) {
    while(1){
        int hasDigit = 0;
        int onlySpaces = 1;
        int i;
        printf("%s", prompt);
        fgets(errorMessage, maxLength, stdin);

        for(i = 0; errorMessage[i] != '\0'; i++){
            if(errorMessage[i] == '\n'){
                errorMessage[i] = '\0';
                break;
            }
        }

        for(i = 0; errorMessage[i] != '\0'; i++){
            if(errorMessage[i] >= '0' && errorMessage[i] <= '9'){
                hasDigit = 1;
                break;
            }
            else if(errorMessage[i] != ' '){
                onlySpaces = 0;
            }
        }

        if(errorMessage[0] == '\0' || onlySpaces){
            printf("Input cannot be empty or just spaces.\n");
        } 
        else if(hasDigit){
            printf("Invalid input. Do not include numbers.\n");
        } 
        else{
            break;
        }
    }
}

int checkValidInt(const char *prompt){
    int value;
    char ch;
    while(1){
        printf("%s", prompt);
        if(scanf("%d", &value) == 1){
            while ((ch = getchar()) != '\n'); 
                if(value < 0){
                 printf("Input cannot be negative. Please enter a valid number.\n");
                } 
                else{
                    return value;
                }
        } 
        else{
            printf("Invalid input. Please enter a number.\n");
            while ((ch = getchar()) != '\n');
        }
    }
}

// Functions to handle employee operations
void addEmployee(){
    int idExists;
    int inputId;
    struct EmployeeData temp;

    do{
        idExists = 0;
        FILE *file = fopen(FILE_NAME, "r");
        inputId = checkValidInt("\nEnter Employee ID: ");

        if(file != NULL){
            while(fscanf(file, "%d\t| %99[^\t]\t| %d\t| %99[^\t]\t| %99[^\t]\t| %d\t| %f\n",&temp.id, temp.name, &temp.age, temp.department, temp.jobPosition, &temp.daysRendered, &temp.salary) == 7){
                if(temp.id == inputId){
                    idExists = 1;
                    printf("Employee ID already exists! Please enter a unique ID.\n");
                    break;
                }
            }
            fclose(file);
        }
    }while(idExists);

    Employee.id = inputId;

    FILE *file = fopen(FILE_NAME, "a");
    if(file == NULL){
        printf("Error opening file for writing.\n");
        return;
    }

    checkValidString("Enter Name: ", Employee.name, 50);
    Employee.age = checkValidInt("Enter Age: ");
    checkValidString("Enter Department: ", Employee.department, 30);
    checkValidString("Enter Position: ", Employee.jobPosition, 20);
    Employee.daysRendered = checkValidInt("Enter Days Rendered: ");
    Employee.salary = getStandardDailyRate(Employee.jobPosition) * Employee.daysRendered;

    fprintf(file, "%d\t| %s\t| %d\t| %s\t| %s\t| %d\t| %.2f\n",Employee.id, Employee.name, Employee.age, Employee.department, Employee.jobPosition, Employee.daysRendered, Employee.salary);
    fclose(file);
    system("cls");
    printf("\nEmployee added successfully!\n");
}

void viewEmployees(){
    system("cls");
    FILE *file = fopen(FILE_NAME, "r");
    int count = 0;
    struct EmployeeData temp;

    if(file == NULL){
        printf("No employee records found.\n");
        return;
    }

    printf("\n==== Employee Records ====\n");
    printf("ID\t| Name\t\t| Age\t| Department\t| Position\t| Days Rendered\t| Salary\n");
    printf("------------------------------------------------------------------------------------------\n");

    while(fscanf(file, "%d\t| %99[^\t]\t| %d\t| %99[^\t]\t| %99[^\t]\t| %d\t| %f\n",&temp.id, temp.name, &temp.age,temp.department, temp.jobPosition,&temp.daysRendered, &temp.salary) == 7){
        printf("%d\t| %s\t| %d\t| %s\t| %s\t| %d\t| %.2f\n",temp.id, temp.name, temp.age,temp.department, temp.jobPosition,temp.daysRendered, temp.salary);
        count++;
    }
    fclose(file);
    printf("------------------------------------------------------------------------------------------\n");
    printf("Total Employees: %d\n", count);
    printf("End of records.\n");
    printf("Press any key to continue...\n");
    getchar();
    getchar();
    system("cls");
    printf("Returning to main menu...\n");
}

void updateEmployee(){
    FILE *file = fopen(FILE_NAME, "r");
    struct EmployeeData temp;
    int found = 0;
    if(!file){
        printf("No employee records found.\n");
        return;
    }

    FILE *tempFile = fopen("Temp.txt", "w");
    if(!tempFile){
        printf("Unable to create temporary file.\n");
        fclose(file);
        return;
    }

    int idToUpdate = checkValidInt("\nEnter Employee ID to update: ");

    while (fscanf(file, "%d\t| %99[^\t]\t| %d\t| %99[^\t]\t| %99[^\t]\t| %d\t| %f\n",&temp.id, temp.name, &temp.age, temp.department, temp.jobPosition,&temp.daysRendered, &temp.salary) == 7){
        if(temp.id == idToUpdate){
            found = 1;
            printf("\nUpdating Employee: %s\n", temp.name);
            checkValidString("Enter new Position: ", temp.jobPosition, 20);
            temp.daysRendered = checkValidInt("Enter new Days Rendered: ");
            temp.salary = getStandardDailyRate(temp.jobPosition) * temp.daysRendered;
            printf("Salary recalculated: %.2f\n", temp.salary);
        }
        fprintf(tempFile, "%d\t| %s\t| %d\t| %s\t| %s\t| %d\t| %.2f\n",temp.id, temp.name, temp.age, temp.department, temp.jobPosition,temp.daysRendered, temp.salary);
    }

    fclose(file);
    fclose(tempFile);

    remove(FILE_NAME);
    rename("Temp.txt", FILE_NAME);

    if(found){
        printf("\nEmployee record updated successfully.\n");
    }
    else{
        printf("\nEmployee with ID %d not found.\n", idToUpdate);
    }

    printf("Press any key to continue...\n");
    getchar();
    system("cls");
}

void deleteEmployee(){
    FILE *file = fopen(FILE_NAME, "r");
    struct EmployeeData temp;
    int found = 0, deleted = 0;
    
    if(!file){
        printf("No employee records found.\n");
        return;
    }

    FILE *tempFile = fopen("Temp.txt", "w");
    if(!tempFile){
        printf("Unable to create temporary file.\n");
        fclose(file);
        return;
    }

    int idToDelete = checkValidInt("Enter Employee ID to delete: ");

    while(fscanf(file, "%d\t| %99[^\t]\t| %d\t| %99[^\t]\t| %99[^\t]\t| %d\t| %f\n",&temp.id, temp.name, &temp.age, temp.department, temp.jobPosition,&temp.daysRendered, &temp.salary) == 7){
        if(temp.id == idToDelete){
            found = 1;
            printf("Found Employee: %s\n", temp.name);
            char confirm;
            printf("Are you sure you want to delete this record? (Y/N): ");
            scanf(" %c", &confirm);
            while (getchar() != '\n');  // flush input buffer
            if(confirm == 'y' || confirm == 'Y'){
                deleted = 1; // Confirm deletion
                continue;    // Skip writing this record
            }
        }
        // Write to temp file (not deleted)
        fprintf(tempFile, "%d\t| %s\t| %d\t| %s\t| %s\t| %d\t| %.2f\n",temp.id, temp.name, temp.age, temp.department, temp.jobPosition,temp.daysRendered, temp.salary);
    }

    fclose(file);
    fclose(tempFile);

    remove(FILE_NAME);
    rename("Temp.txt", FILE_NAME);
    
    if(found && deleted){
        printf("\nEmployee record deleted successfully.\n");
    }
    else if(found && !deleted){
        printf("\nDeletion canceled. Employee record not deleted.\n");
    }
    else{
        printf("\nEmployee with ID %d not found.\n", idToDelete);
    }

    printf("Press any key to continue...\n");
    getchar();
    system("cls");
}

void searchSortEmployees() {
    FILE *file = fopen(FILE_NAME, "r");
    char choice[10];
    int found = 0;
    if(!file){
        printf("\nNo employee records found. Please add employees first.\n");
        return;
    }

    struct EmployeeData employees[100];
    int count = 0;

    while(fscanf(file, "%d\t| %99[^\t]\t| %d\t| %99[^\t]\t| %99[^\t]\t| %d\t| %f\n",&employees[count].id, employees[count].name, &employees[count].age,employees[count].department, employees[count].jobPosition,&employees[count].daysRendered, &employees[count].salary) == 7){
        count++;
    }
    fclose(file);

    if(count == 0){
        printf("\nNo employee data to display.\n");
        return;
    }


    printf("\nSearch or Sort Employees:\n");
    printf("1. Search by Name\n");
    printf("2. Search by Department\n");
    printf("3. Sort by Name (A-Z)\n");
    printf("4. Sort by Salary (High to Low)\n");
    printf("Enter choice: ");
    scanf("%s", choice);
    while(getchar() != '\n');

    if(choice[0] == '1' || choice[0] == '2') {
        char keyword[100];
        printf("Enter search keyword: ");
        fgets(keyword, sizeof(keyword), stdin);
        keyword[strcspn(keyword, "\n")] = '\0';

        if(strlen(keyword) == 0 || strspn(keyword, " ") == strlen(keyword)) {
            printf("Search keyword cannot be empty or just spaces.\n");
            return;
        }
        for(int i = 0; i < count; i++) {
            int isMatch = 0;
            const char *searchField = (choice[0] == '1') ? employees[i].name : employees[i].department;
            if(strlen(keyword) == 0) {
            isMatch = 1;
            }
            else{
                for(const char *p = searchField; *p && !isMatch; p++) {
                    const char *h = p;
                    const char *k = keyword;
                    while(*h && *k && (tolower((unsigned char)*h) == tolower((unsigned char)*k))){
                        h++;
                        k++;
                    }
                    if(!*k){
                        isMatch = 1;
                    }
                }
            }
            if(isMatch){
                printf("%d\t| %s\t| %d\t| %s\t| %s\t| %d\t| %.2f\n", employees[i].id, employees[i].name, employees[i].age, employees[i].department, employees[i].jobPosition, employees[i].daysRendered, employees[i].salary);
                found = 1;
            }
        }

        if(!found){
            printf("\nNo matching records found.\n");
        }
    }
    else if(choice[0] == '3'){
        for(int i = 0; i < count - 1; i++){
            for(int j = i + 1; j < count; j++) {
                if(strcasecmp(employees[i].name, employees[j].name) > 0){
                    struct EmployeeData temp = employees[i];
                    employees[i] = employees[j];
                    employees[j] = temp;
                }
            }
        }
        printf("\nSorted by Name (A-Z):\n");
        for(int i = 0; i < count; i++) {
            printf("%d\t| %s\t| %d\t| %s\t| %s\t| %d\t| %.2f\n", employees[i].id, employees[i].name, employees[i].age, employees[i].department, employees[i].jobPosition, employees[i].daysRendered, employees[i].salary);
        }

    } 
    else if(choice[0] == '4'){
        for(int i = 0; i < count - 1; i++){
            for(int j = i + 1; j < count; j++) {
                if(employees[i].salary < employees[j].salary) {
                    struct EmployeeData temp = employees[i];
                    employees[i] = employees[j];
                    employees[j] = temp;
                }
            }
        }
        printf("\nSorted by Salary (High to Low):\n");
        for(int i = 0; i < count; i++) {
            printf("%d\t| %s\t| %d\t| %s\t| %s\t| %d\t| %.2f\n", employees[i].id, employees[i].name, employees[i].age, employees[i].department, employees[i].jobPosition, employees[i].daysRendered, employees[i].salary);
        }
    }
    else{
        printf("Invalid option. Please enter a valid number (1-4).\n");
    }

    printf("\nPress Enter to return to the main menu...");
    getchar();
    system("cls");
}

void computeSalary() {
    struct EmployeeData employees[100];
    int count = 0;
    FILE *file = fopen(FILE_NAME, "r");

    if(!file){
        printf("No employee records found.\n");
        return;
    }

    // Read all employees into array
    while(fscanf(file, "%d\t| %99[^\t]\t| %d\t| %99[^\t]\t| %99[^\t]\t| %d\t| %f\n",&employees[count].id, employees[count].name, &employees[count].age,employees[count].department, employees[count].jobPosition,&employees[count].daysRendered, &employees[count].salary) == 7){
        count++;
    }
    fclose(file);

    int idToCompute = checkValidInt("Enter Employee ID to compute salary: ");
    int found = 0;

    for(int i = 0; i < count; i++){
        if(employees[i].id == idToCompute){
            float dailyRate = getStandardDailyRate(employees[i].jobPosition);
            employees[i].salary = dailyRate * employees[i].daysRendered;

            // PAYSLIP
            printf("\n======= Payslip for %s =======\n", employees[i].name);
            printf("Position       : %s\n", employees[i].jobPosition);
            printf("Days Rendered  : %d\n", employees[i].daysRendered);
            printf("Daily Rate     : PHP %.2f\n", dailyRate);
            printf("Total Salary   : PHP %.2f\n", employees[i].salary);
            found = 1;
            break;
        }
    }

    if(!found){
        printf("Employee with ID %d not found.\n", idToCompute);
        printf("Press any key to return to menu...\n");
        getchar();
        system("cls");
        return;
    }

    file = fopen(FILE_NAME, "w");
    if(!file){
        printf("Error saving updated salaries.\n");
        return;
    }

    for(int i = 0; i < count; i++){
        fprintf(file, "%d\t| %s\t| %d\t| %s\t| %s\t| %d\t| %.2f\n",employees[i].id, employees[i].name, employees[i].age,employees[i].department, employees[i].jobPosition,employees[i].daysRendered, employees[i].salary);
    }

    fclose(file);
    printf("\nSalary computed and saved for employee ID %d.\n", idToCompute);
    printf("Press any key to return to menu...\n");
    getchar();
    system("cls");
}

//main function to handle the menu and file operations
int main(){
    char choice[10];

    do{
        printf("==== Employee Management System ====\n");
        printf("1. Add New Employee\n");
        printf("2. View All Employees\n");
        printf("3. Update Employee Details\n");
        printf("4. Delete Employee Record\n");
        printf("5. Search / Sort Employees\n");
        printf("6. Compute Employee Salary\n");
        printf("7. Exit\n");
        printf("Enter your choice: ");

        scanf("%s", &choice);

        switch(choice[0]){
            case '1':
                addEmployee();
                break;
            case '2':
                viewEmployees();
                break;
            case '3':
                updateEmployee();
                break;
            case '4':
                deleteEmployee();
                break;
            case '5':
                searchSortEmployees();
                break;
            case '6':
                computeSalary();
                break;
            case '7':
                printf("Exiting the program. Goodbye!\n");
                break;
            default:
                printf("Invalid choice! Please try again.\n");
                break;
        }
    }while(choice[0] != '7');
    return 0;
}