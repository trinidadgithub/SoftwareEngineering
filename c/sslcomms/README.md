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
