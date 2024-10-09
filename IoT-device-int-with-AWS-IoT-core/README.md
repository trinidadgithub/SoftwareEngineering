# IoT Device Integration with AWS IoT Core and MQTT

**Focus**: Real-time networking, efficient data transmission via MQTT, and performance monitoring using AWS CloudWatch, with Terraform for automated infrastructure management.

## Overview

This project demonstrates how to deploy an IoT solution using AWS IoT Core for managing devices and the MQTT protocol for real-time, bidirectional communication. The infrastructure is managed using Terraform, making it scalable and easily maintainable. Python is used to implement the MQTT client that facilitates communication between IoT devices and the cloud.

## Technologies Used
- AWS IoT Core
- MQTT
- Python
- AWS IAM (Identity and Access Management)
- AWS S3 (optional)
- AWS DynamoDB (optional)
- AWS CloudWatch
- Terraform
- X.509 Certificates for secure communication

## Directory Structure

```bash
iot-aws-mqtt-solution/
│
├── terraform/
│   ├── main.tf                  # Main Terraform configuration
│   ├── variables.tf             # Terraform variables
│   ├── outputs.tf               # Outputs for relevant resources
│   ├── provider.tf              # AWS provider configuration
│   ├── modules/
│   │   ├── iot-core/            # IoT Core resources module
│   │   └── iam/                 # IAM roles and policies module
│   └── data/                    # Optional: IoT policy JSON or CA certs
│
├── python/                      # Python MQTT client scripts
│   ├── mqtt_client.py           # Python code for MQTT communication
│   ├── requirements.txt         # Python dependencies (paho-mqtt)
│   └── certificates/            # AWS IoT device certificates (CA, device certs, keys)
│       ├── AmazonRootCA1.pem    
│       ├── device-certificate.pem.crt
│       └── device-private.pem.key
│
├── logs/                        # Logs from Python MQTT client (optional)
│   └── mqtt_client.log
│
└── README.md                    # Project documentation
```

## Prerequisites

1. **AWS Account**: You’ll need access to an AWS account with permissions to create and manage IoT Core, IAM, S3, and CloudWatch resources.
2. **Terraform**: Ensure Terraform is installed and configured with your AWS credentials.
3. **Python**: You’ll need Python 3.x installed along with the required libraries specified in `requirements.txt`.
4. **MQTT Client Certificates**: AWS IoT Core will provide the device certificates required for secure communication.

## Setup Instructions

### 1. Set up Terraform for AWS IoT Core

1. Navigate to the `terraform/` directory:
    ```bash
    cd terraform/
    ```

2. Initialize Terraform:
    ```bash
    terraform init
    ```

3. Customize the variables in `variables.tf` (if any).

4. Run a Terraform plan to see what resources will be created:
    ```bash
    terraform plan
    ```

5. Apply the Terraform configuration to provision AWS resources:
    ```bash
    terraform apply
    ```

### 2. Python MQTT Client

1. Navigate to the `python/` directory:
    ```bash
    cd python/
    ```

2. Install Python dependencies:
    ```bash
    pip install -r requirements.txt
    ```

3. Ensure your MQTT client certificates (device and CA) are in the `certificates/` folder.

4. Run the MQTT client:
    ```bash
    python mqtt_client.py
    ```

### 3. Monitoring with AWS CloudWatch

The solution is integrated with AWS CloudWatch for logging and monitoring IoT device communication. You can monitor device status, message exchanges, and performance metrics in CloudWatch.

## Project Customization

- **IoT Policies**: You can update or add IoT policies in the `terraform/modules/iot-core/` directory to customize how devices interact with AWS IoT Core.
- **MQTT Topics**: The topics for publishing/subscribing can be customized in the `mqtt_client.py` file to match your specific use case.
- **Logging**: Python logging can be enhanced in `mqtt_client.py` to capture more details, and logs can be redirected to files in the `logs/` folder.

## Optional Components

- **AWS S3**: If you want to store IoT data, add S3 bucket resources to the Terraform configuration.
- **AWS DynamoDB**: For storing device metadata, you can include DynamoDB resources.

## License

This project is licensed under the MIT License.

---

## Next Steps

1. **Deploy and Test**: After setting up and deploying the infrastructure and Python client, test the device communication by publishing and subscribing to topics.
2. **Expand Functionality**: Add additional IoT devices or integrate with other AWS services like AWS Lambda for further processing of IoT data.

