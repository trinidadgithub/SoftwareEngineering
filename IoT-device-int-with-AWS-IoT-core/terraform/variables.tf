# Define variables for the Terraform configuration

variable "iot_device_name" {
  description = "Name of the IoT device"
  type        = string
  default     = "MyIoTDevice"
}

variable "region" {
  description = "AWS region to deploy resources"
  type        = string
  default     = "us-west-2"
}
