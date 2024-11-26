## Simple Database in C

This code implements a simple REPL (Read-Eval-Print Loop) for a database-like interactive program.  The inspiration comes from here:  https://cstack.github.io/db_tutorial/parts/part1.html.  Let's break it down in detail:  

---
### Features

- Insert Command: Adds a new row to the database.

- Select Command: Retrieves all rows from the database.

- Error Handling:

  - Syntax errors.

  - Table capacity exceeded.

- Meta Commands:

  - .exit: Exits the program.
---
## 2. Build and Run 
**Prerequisites**
- GCC or any C Compiler
- Linux or macOS terminal (or equivalent environment).  

**Build**  
Compile the program using `gcc`:
```bash
gcc db.c -o db
```
**Run**
Start the program
```bash
./db
```
You should see the prompt:
```bash
db >
```
## Usage Examples
1. Insert Rows  
   Add new rows to the database using the `insert` command:
    ```bash
    db > insert 1 user1 user1@example.com
    Executed.

    db > insert 2 user2 user2@example.com
    Executed.  
    ```
2. Retrieve Rows  
   Fetch all rows using the `select` command:
   ```bash
   db > select
   (1, user1, user1@example.com)
   (2, user2, user2@example.com)
   Executed.
   ```
3. Exit the Program  
   Exit the REPL with the .exit command:
   ```bash
   db > .exit
   ```

---
## Error Handling  
1. Syntax Error:
    ```bash
    db > insert 3 user3
    Syntax error. Could not parse statement.
    ```
2. Unrecognized Commands:
   ```bash
   db > unknown_command
   Unrecognized keyword at start of 'unknown_command'.
   ```
3. Table Full:
   ```bash
   db > insert 1401 user1401 user1401@example.com
   Error: Table full.
   ```

---
## Code Structure
Main Functions  
1. prepare_statement  
   Prepares the input statement for execution.

    ```bash
    PrepareResult prepare_statement(InputBuffer* input_buffer, Statement* statement) {
    if (strncmp(input_buffer->buffer, "insert", 6) == 0) {
        statement->type = STATEMENT_INSERT;
        int args_assigned = sscanf(
            input_buffer->buffer, "insert %d %31s %254s",
            &(statement->row_to_insert.id),
            statement->row_to_insert.username,
            statement->row_to_insert.email
        );
        if (args_assigned < 3) {
            return PREPARE_SYNTAX_ERROR;
        }
        return PREPARE_SUCCESS;
    }
    if (strcmp(input_buffer->buffer, "select") == 0) {
        statement->type = STATEMENT_SELECT;
        return PREPARE_SUCCESS;
    }
    return PREPARE_UNRECOGNIZED_STATEMENT;
    ```
2. execute_insert  
   ```bash
   ExecuteResult execute_insert(Statement* statement, Table* table) {
    if (table->num_rows >= TABLE_MAX_ROWS) {
        return EXECUTE_TABLE_FULL;
    }

    Row* row_to_insert = &(statement->row_to_insert);
    serialize_row(row_to_insert, row_slot(table, table->num_rows));
    table->num_rows += 1;

    return EXECUTE_SUCCESS;
   ```
3. execute_select  
Executes the `select` command.
   ```bash
   ExecuteResult execute_select(Statement* statement, Table* table) {
    for (uint32_t i = 0; i < table->num_rows; i++) {
        Row row;
        deserialize_row(row_slot(table, i), &row);
        printf("(%d, %s, %s)\n", row.id, row.username, row.email);
    }
    return EXECUTE_SUCCESS;
   ```
4. row_slot  
   Calculates the memory location of a row in the table.
   ```bash
   void* row_slot(Table* table, uint32_t row_num) {
    uint32_t page_num = row_num / ROWS_PER_PAGE;
    void* page = table->pages[page_num];
    if (page == NULL) {
        page = table->pages[page_num] = malloc(PAGE_SIZE);
    }
    uint32_t row_offset = row_num % ROWS_PER_PAGE;
    uint32_t byte_offset = row_offset * ROW_SIZE;
    return page + byte_offset;
   ```
---
## Constants
Memory Layout  
- ID Size: 4 bytes
- Username Size: 32 bytes
- Email Size: 255 bytes
- Row Size: 291 bytes
- Page Size: 4096 bytes
---
## Future Enhancements
1. Persistence:
   - Add file-based storage for saving and retrieving data between sessions.
2. Indexing:
   - Implement indexing for faster `select` operations.
3. Transactions:
   - Support atomic operations for data consistency.