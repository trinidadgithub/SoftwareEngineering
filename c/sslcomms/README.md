Key Features:

    - Error Handling: The check_success function checks if each openssl command was successful. If not, it logs the error and exits the script.
    - Logging: Each significant action is logged to a file named generate_certs.log for tracking purposes.
    - Functions: 
        log: Logs messages with a timestamp to a log file.
        check_success: Checks the exit status of the last command.
        generate_cert: A reusable function to generate certificates, reducing code duplication.
    - Checks: 
        Checks if OpenSSL is installed before attempting to use it.
    Modularity: By using functions, the script becomes more modular and easier to maintain or extend.


Usage:

    Save this script to a file, for example generate_certs.sh.
    Make it executable with chmod +x generate_certs.sh.
    Run it with ./generate_certs.sh.


This script will provide feedback on what's happening during the certificate generation process, and it will fail gracefully if something goes wrong, informing the user of the specific step where the failure occurred.

## Adding check file feature

Key Enhancements:

    File Existence Check: The check_file function checks if a file exists and prompts the user for permission to overwrite it. 
    Conditional Certificate Generation: The generate_cert function now checks if each certificate file already exists before proceeding with generation. If any file exists and the user opts not to overwrite, the certificate generation for that type is skipped.
    User Interaction: The script now interacts with the user to confirm overwriting existing files, providing an extra layer of caution and control.


Usage:

    Ensure to save this script with the .sh extension, make it executable (chmod +x generate_certs.sh), and run it (./generate_certs.sh).


This setup ensures that you can safely use the script without accidentally overwriting important existing certificates, giving you the control to decide whether to regenerate certificates or not.

### Separating directory structure
Key Enhancements:

    Directory Variables: CA_DIR and CERTS_DIR are now variables at the top of the script, making it easy to change the storage locations for certificates.
    Directory Creation: The script now checks to ensure the directories exist, creating them if they do not.
    File Paths: All file paths in the functions are now constructed using these variables, ensuring that the CA's keys, CSRs, and CRT are stored in the ca directory, while the server and client certificates are placed in the certs directory.


This script is now more flexible for different directory structures and includes better error handling and logging.


