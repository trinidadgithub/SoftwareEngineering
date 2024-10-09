# AWS IoT Core Thing creation
resource "aws_iot_thing" "my_iot_device" {
  name = var.iot_device_name
}

# AWS IoT Policy
resource "aws_iot_policy" "iot_policy" {
  name = "IoTCorePolicy"
  policy = <<EOF
{
  "Version": "2012-10-17",
  "Statement": [
    {
      "Effect": "Allow",
      "Action": [
        "iot:Connect",
        "iot:Publish",
        "iot:Subscribe",
        "iot:Receive"
      ],
      "Resource": "*"
    }
  ]
}
EOF
}

# AWS IoT Certificate
resource "aws_iot_certificate" "device_certificate" {
  active = true
}

# AWS IAM Role for IoT Core
resource "aws_iam_role" "iot_device_role" {
  name = "iot_device_role"

  assume_role_policy = <<EOF
{
  "Version": "2012-10-17",
  "Statement": [
    {
      "Effect": "Allow",
      "Principal": {
        "Service": "iot.amazonaws.com"
      },
      "Action": "sts:AssumeRole"
    }
  ]
}
EOF
}
