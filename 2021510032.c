#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xpath.h>
#include <libxml/xmlschemastypes.h>

// Code works when
// gcc 2021510032.c -o 2021510032 -I/usr/include/libxml2 -lxml2
// is written on terminal, code does not take any command line and does all convertions at once

#define MAX_LINE_LENGTH 150
// Define a struct to hold information about a person
typedef struct
{
    char name[20];
    char surname[30];
    char gender[6];
    char occupancy[30];
    char level_of_education[30];
    char email[30];
    char bank_account_number[30];
    char IBAN[30];
    char account_type[13];
    char currency_unit[13];
    char total_balance_available[30];
    char available_for_loan[30];
} Person;
// This function converts a little-endian integer to a big-endian integer
int convertLittleToBigEndian(int littleEndianValue)
{
    int bigEndianValue = 0;
    for (int i = 0; i < sizeof(int); i++)
    {
        bigEndianValue |= ((littleEndianValue >> (8 * i)) & 0xFF) << ((sizeof(int) - 1 - i) * 8);
    }
    return bigEndianValue;
}
// This function is a custom implementation of strtok() that takes a string and a delimiter
// It returns a pointer to the next token in the string, or NULL if no more tokens are found

char *strtoke(char *str, const char *delim)
{
    static char *start = NULL; /* stores string str for consecutive calls */
    char *token = NULL;        /* found token */
    // If str is not NULL, set start to point to the beginning of the string
    if (str)
        start = str;
    // If start is NULL, return NULL
    if (!start)
        return NULL;
     // Set token to start
    token = start;
    // Find the next occurrence of the delimiter in the string
    start = strpbrk(start, delim);
    // If the delimiter was found, replace it with a null terminator and move start to follower
    if (start)
        *start++ = '\0';
    //return the token
    return token;
}
// This function writes an array of Person structs to a binary file
// It returns true if the write was successful
bool write_data(char *filename, Person *data, int total)
{
    // file pointer variable
    FILE *file;

    // attempt to open the file with name filename, in 'write to binary file mode'
    file = fopen(filename, "wb");

    // return false if there was an error opening the file
    if (file == NULL)
        return false;

    // write the total number of structs in the array to the file, return false
    // if the function fails to write the data successfully
    if (fwrite(&total, sizeof(int), 1, file) != 1)
        return false;

    // write the structs in the array to the file, return false if the function
    // fails to write the data successfully
    if (fwrite(data, sizeof(Person), total, file) != total)
        return false;

    // close access to the file, return false if this fails
    if (fclose(file) == EOF)
        return false;

    // if everything is successful return true
    return true;
}
// function to read an array of Person structs from a binary file
Person *read_data(char *filename, int *total)
{
    FILE *file;

    // open the file with name filename in 'read a binary file mode'
    file = fopen(filename, "rb");

    // if fopen() failed to open the file, return NULL
    if (file == NULL)
        return NULL;

    // read the total number of Student struct data records stored in the file
    // into the total pointer parameter
    if (fread(total, sizeof(int), 1, file) != 1)
        return NULL;

    // allocate enough space to store the array of Student structs
    Person *data = malloc(sizeof(Person) * *total);

    // read the data from the file into the block of memory we have allocated,
    // return NULL if the read was unsuccessful and free the dynamically allocated
    // memory to prevent a memory leak
    if (fread(data, sizeof(Person), *total, file) != *total)
    {
        free(data);
        return NULL;
    }
    // Close the file
    if (fclose(file) == EOF)
    {
        free(data);
        return NULL;
    }
    return data;
}
int main()
{
 // open csv and binary files
    FILE *csvFile = fopen("records.csv", "r");
    FILE *binaryFile = fopen("records.dat", "wb");

    char line[MAX_LINE_LENGTH];
    char *token;
    // allocate memory for Person array
    Person *person;
    person = malloc(sizeof(Person) * 51);
// check if files were opened successfully
    if (csvFile == NULL || binaryFile == NULL)
    {
        // if not, print an error message 
        puts("Error occured while opening file\n");
        exit(1);
    }
    int i = 0;
     // read from csv file line by line
    while (fgets(line, MAX_LINE_LENGTH, csvFile) != NULL)
    {
        // Parse the line and fill in the person struct
        if (i != 0)
        {
            // read each token separated by commas from the line
            token = strtoke(line, ",");
            if (token != NULL)
            {
                // if token exists, copy it into person's name field
                strcpy(person[i].name, token);
            }

            /* repeat the above process for surname, gender, occupancy, level of education, email, 
            bank account number, IBAN, account type, currency unit, total balance available, and
            available for loan*/
            token = strtoke(NULL, ",");
            if (token != NULL)
            {
                strcpy(person[i].surname, token);
            }
            token = strtoke(NULL, ",");
            if (token != NULL)
            {
                strcpy(person[i].gender, token);
            }
            token = strtoke(NULL, ",");
            if (token != NULL)
            {
                strcpy(person[i].occupancy, token);
            }
            token = strtoke(NULL, ",");
            if (token != NULL)
            {
                strcpy(person[i].level_of_education, token);
            }
            token = strtoke(NULL, ",");
            if (token != NULL)
            {
                strcpy(person[i].email, token);
            }
            token = strtoke(NULL, ",");
            if (token != NULL)
            {
                strcpy(person[i].bank_account_number, token);
            }
            token = strtoke(NULL, ",");
            if (token != NULL)
            {
                strcpy(person[i].IBAN, token);
            }
            token = strtoke(NULL, ",");
            if (token != NULL)
            {
                strcpy(person[i].account_type, token);
            }
            token = strtoke(NULL, ",");
            if (token != NULL)
            {
                strcpy(person[i].currency_unit, token);
            }
            token = strtoke(NULL, ",");
            if (token != NULL)
            {
                strcpy(person[i].total_balance_available, token);
            }
            token = strtoke(NULL, ",");
            if (token != NULL)
            {
                strcpy(person[i].available_for_loan, token);
            }
        }
        // increment i to move to the next person
        i++;
        // Write the person struct to the binary file
        /// fwrite(&person, sizeof(Person), 1, binaryFile);
    }
    // call the write_data function to write the person array to a binary
    if (write_data("records.dat", person, 51))
    {
        puts("Convertion from csv to binary completed");
    }
    // free memory allocated for person array and close both csvFile and binaryFile
    free(person);
    fclose(csvFile);
    fclose(binaryFile);
    // total and file_data variables and read data from "records.dat" binary file
    int total = 0;
    Person *file_data;
    file_data = read_data("records.dat", &total);

    // if the read was unsuccessful, report the error and exit with error status
    if (file_data == NULL)
    {
        printf("Error reading from file.\n");
        return 1;
    }
    else
    {
        // If file_data is not NULL
        puts("Read from binary completed");
    }

    // Create an XML document
    xmlDocPtr doc = xmlNewDoc(BAD_CAST "1.0");
    xmlNodePtr root = xmlNewNode(NULL, BAD_CAST "records");
    xmlDocSetRootElement(doc, root);
    // for loop through the file_data array and add each record to the XML document
    for (int i = 0; i < total; i++)
    {
        // convert the current row number to a string
        char row[20];
        sprintf(row, "%d", i);
        if (i > 0)
        {
            // If this is not the first record, add a row_id node to the XML document
            xmlNodePtr item0 = xmlNewChild(root, NULL, BAD_CAST "row_id", NULL);
            xmlSetProp(item0, BAD_CAST "row_id", BAD_CAST row);

            // Add a customer_info node to the row_id node
            xmlNodePtr item = xmlNewChild(item0, NULL, BAD_CAST "customer_info", NULL);

            // Add nodes for each of the customer's info
            xmlNodePtr name = xmlNewChild(item, NULL, BAD_CAST "name", NULL);
            xmlNodeSetContent(name, BAD_CAST file_data[i].name);

            // Add the person's surname
            xmlNodePtr surname = xmlNewChild(item, NULL, BAD_CAST "surname", NULL);
            xmlNodeSetContent(surname, BAD_CAST file_data[i].surname);

            // Add the person's gender
            xmlNodePtr gender = xmlNewChild(item, NULL, BAD_CAST "gender", NULL);
            xmlNodeSetContent(gender, BAD_CAST BAD_CAST file_data[i].gender);
            // Add the person's occupancy
            xmlNodePtr occupancy = xmlNewChild(item, NULL, BAD_CAST "occupancy", NULL);
            xmlNodeSetContent(occupancy, BAD_CAST file_data[i].occupancy);
            // Add the person's education level
            xmlNodePtr level_of_education = xmlNewChild(item, NULL, BAD_CAST "level_of_education", NULL);
            xmlNodeSetContent(level_of_education, BAD_CAST file_data[i].level_of_education);
            // Add the person's email address
            xmlNodePtr email = xmlNewChild(item, NULL, BAD_CAST "email", NULL);
            xmlNodeSetContent(email, BAD_CAST file_data[i].email);

        // Create XML elements for bank information
            xmlNodePtr item1 = xmlNewChild(item0, NULL, BAD_CAST "bank_info", NULL);

            // Add the person's bank account number
            xmlNodePtr bank_account_number = xmlNewChild(item1, NULL, BAD_CAST "bank_account_number", NULL);
            xmlNodeSetContent(bank_account_number, BAD_CAST file_data[i].bank_account_number);

            // Add the person's IBAN
            xmlNodePtr count = xmlNewChild(item1, NULL, BAD_CAST "IBAN", NULL);
            xmlNodeSetContent(count, BAD_CAST file_data[i].IBAN);

            // Add the person's account type
            xmlNodePtr IBAN = xmlNewChild(item1, NULL, BAD_CAST "account_type", NULL);
            xmlNodeSetContent(IBAN, BAD_CAST file_data[i].account_type);

            // Add the person's currency unit
            xmlNodePtr currency_unit = xmlNewChild(item1, NULL, BAD_CAST "currency_unit", NULL);
            xmlNodeSetContent(currency_unit, BAD_CAST file_data[i].currency_unit);

            // Add the person's total available balances
            xmlNodePtr total_balance_available = xmlNewChild(item1, NULL, BAD_CAST "total_balance_available", NULL);
            ;
            xmlNodeSetContent(total_balance_available, BAD_CAST file_data[i].total_balance_available);

        // Convert the person's total balance available to big endian and add to the XML
            char end[50];
            sprintf(end, "%d", convertLittleToBigEndian(atoi(file_data[i].total_balance_available)));
            // Add the person's total available balance in big endian format
            xmlNodePtr big_endian = xmlNewChild(item1, NULL, BAD_CAST "big_endian", NULL);
            xmlNodeSetContent(big_endian, BAD_CAST end);

            // Add the person's available for loan
            xmlNodePtr available_for_loan = xmlNewChild(item1, NULL, BAD_CAST "available_for_loan", NULL);
            xmlNodeSetContent(available_for_loan, BAD_CAST file_data[i].available_for_loan);
        }
    }

    // Write the XML document to a file
    xmlSaveFormatFileEnc("records.xml", doc, "UTF-8", 1);
    puts("Convertion from binary to xml completed");
    // free the dynamically allocated memory
    free(file_data);
    // XML file against its corresponding XSD schema file
    xmlSchemaPtr schema = NULL;
    xmlSchemaParserCtxtPtr ctxt;

    char *XMLFileName = "records.xml"; // write your xml file here
    char *XSDFileName = "2021510032.xsd"; // write your xsd file here

    xmlLineNumbersDefault(1);                   // set line numbers, 0> no substitution, 1>substitution
    ctxt = xmlSchemaNewParserCtxt(XSDFileName); // create an xml schemas parse context
    schema = xmlSchemaParse(ctxt);              // parse a schema definition resource and build an internal XML schema
    xmlSchemaFreeParserCtxt(ctxt);              // free the resources associated to the schema parser context

    doc = xmlReadFile(XMLFileName, NULL, 0); // parse an XML file

    // check if parsing was successful
    if (doc == NULL)
    {
        fprintf(stderr, "Could not parse %s\n", XMLFileName);
    }
    else
    {
        xmlSchemaValidCtxtPtr ctxt; // structure xmlSchemaValidCtxt, not public by API
        int ret;

        ctxt = xmlSchemaNewValidCtxt(schema);  // create an xml schemas validation context
        ret = xmlSchemaValidateDoc(ctxt, doc); // validate a document tree in memory
         // check if validation was successful
        if (ret == 0)                         
        {
            printf("%s validates\n", XMLFileName);
        }
        else if (ret > 0) // positive error code number
        {
            printf("%s fails to validate\n", XMLFileName);
        }
        else // internal or API error
        {
            printf("%s validation generated an internal error\n", XMLFileName);
        } 
        // free the resources associated to the schema validation context
        xmlSchemaFreeValidCtxt(ctxt);
        xmlFreeDoc(doc);
    }
    //clean up the default XML schema types library, and free the memory allocated by the library itself
    if (schema != NULL)
        xmlSchemaFree(schema); // deallocate a schema structure

    xmlSchemaCleanupTypes(); // cleanup the default xml schemas types library
    xmlCleanupParser();      // cleans memory allocated by the library itself
    xmlMemoryDump();         // memory dump
    //program has completed successfully
    return 0;
}