# Full-Stack Web Application

This project is a full-featured web application that showcases both front-end and back-end development skills using **React**, **Node.js**, **PostgreSQL**, and **Docker**. The application also integrates a CI/CD pipeline using **GitHub Actions** for streamlined testing and deployment.

## Project Overview

The web application demonstrates a comprehensive full-stack development approach. It uses **React** for the front-end, **Node.js** for the back-end API, **PostgreSQL** for data storage, and is containerized with **Docker** to ensure consistent development and deployment environments. The project is built with scalability and maintainability in mind and integrates continuous integration and deployment through **GitHub Actions**.

### Key Features:
- **Front-End:** Developed using React to create a responsive and dynamic user interface.
- **Back-End:** Node.js with Express to manage API requests and business logic.
- **Database:** PostgreSQL database with a structured schema, handling CRUD operations.
- **Dockerized:** Application is containerized to run in any environment consistently.
- **CI/CD Pipeline:** GitHub Actions is used for continuous integration, running tests, building Docker images, and automating deployments.

## Project Structure

```bash
├── backend
│   ├── Dockerfile             # Backend service container configuration
│   ├── package.json           # Backend dependencies and scripts
│   ├── server.js              # Main entry point for the backend (Express server)
│   └── __tests__              # Unit tests for the backend
│       └── app.test.js
├── create_tf_structure.sh      # Terraform setup script (if applicable)
├── database
│   ├── Dockerfile             # Database container configuration
│   └── init.sql               # SQL file for database initialization
├── docker-compose.yml          # Docker Compose for local development
├── docs
│   └── documentation.md       # Project documentation
├── frontend
│   ├── Dockerfile             # Frontend service container configuration
│   ├── package.json           # Frontend dependencies and scripts
│   ├── package-lock.json      # Lock file for frontend dependencies
│   ├── public
│   │   └── index.html         # Main HTML file for the frontend
│   └── src
│       ├── App.js             # Main React application component
│       ├── index.css          # Global CSS for the frontend
│       └── index.js           # Entry point for the React application
├── .github/workflows
│   └── ci-cd.yml              # CI/CD pipeline configuration
├── LICENSE                    # License file
└── README.md                  # Project documentation (this file)

```
### Folder Explanations:

- **backend/**: Contains all back-end-related code and configurations, including unit tests, the Express server, and the Dockerfile for containerization.
- **database/**: Includes database initialization scripts and Docker configuration for PostgreSQL.
- **frontend/**: Houses the front-end React application, structured into components, with a Dockerfile for deployment.
- **docs/**: Holds additional project documentation files, including detailed setup or design notes.
- **.github/workflows/**: Contains CI/CD pipeline configuration files using GitHub Actions.
- **docker-compose.yml**: Manages multi-container Docker applications, setting up and linking the backend, frontend, and database services.

### Technologies

- **React**: Front-end JavaScript library for building user interfaces.
- **Node.js**: Back-end JavaScript runtime for creating APIs and server-side logic.
- **PostgreSQL**: Open-source relational database.
- **Docker**: Containerization platform to ensure consistent environments.
- **CI/CD**: GitHub Actions for automated testing and deployment.

### Getting Started

### Prerequisites

Before you begin, ensure you have the following installed:

- [Docker](https://www.docker.com/) (latest version)
- [Node.js](https://nodejs.org/) (for development purposes)
- [PostgreSQL](https://www.postgresql.org/) (installed locally or use Docker for the included setup)

#### Setup Instructions

1. **Clone the Repository**
```bash
   git clone https://github.com/your-username/fullstack-web-app.git
   cd fullstack-web-app
```
2. **Start the Application with Docker Compose** To run all services (frontend, backend, and database) together:
```bash
docker compose up --build
```

Note:  This command will build and run all services (frontend, backend, and PostgreSQL database) in Docker containers.

3. **Access the Application**

- **Frontend**: [http://localhost:3000](http://localhost:3000)
- **Backend**: [http://localhost:5000/api](http://localhost:5000/api)
- **PostgreSQL**: Connect to the database on `localhost:5432`

### Environment Variables

Create a .env file in the backend/ directory to configure environment variables for the server:
```bash
PORT=5000
DATABASE_URL=postgres://username:password@localhost:5432/database_name
JWT_SECRET=your_jwt_secret
```

Running Tests

To run unit tests for the back-end:
```bash
# To test locally
docker compose run backend npm test
```
This command runs the test suite defined in the __tests__/ folder.

### CI/CD Pipeline

This project integrates CI/CD pipelines using **GitHub Actions**. The pipeline will:

- Run tests
- Lint the code, currently only setup for frontend as an example.
- Build the Docker images
- Deploy the application (if set up for deployment)

You can find the pipeline configuration in `.github/workflows/ci-cd.yml`.

### Deployment

To deploy the application to a production environment, use the production-ready Docker Compose configuration:
```bash
docker-compose -f docker-compose.prod.yml up --build
```

### License

This project is licensed under the MIT License - see the [LICENSE](./LICENSE) file for details.



