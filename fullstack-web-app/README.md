# Full-Stack Web Application

This project is a full-featured web application that showcases both front-end and back-end development skills using **React**, **Node.js**, **PostgreSQL**, and **Docker**. The application also integrates a CI/CD pipeline using **GitHub Actions**.

## Project Structure

```bash
fullstack-web-app/
├── backend/
│   ├── Dockerfile
│   ├── server.js
│   └── package.json
├── frontend/
│   ├── Dockerfile
│   ├── src/
│   │   ├── App.js
│   │   ├── index.js
│   └── package.json
├── database/
│   ├── Dockerfile
│   └── init.sql
├── docker-compose.yml
├── README.md
└── .github/
    └── workflows/
        └── ci-cd.yml
```

### Technologies

- **React**: Front-end JavaScript library for building user interfaces.
- **Node.js**: Back-end JavaScript runtime for creating APIs and server-side logic.
- **PostgreSQL**: Open-source relational database.
- **Docker**: Containerization platform to ensure consistent environments.
- **CI/CD**: GitHub Actions for automated testing and deployment.

### Getting Started

#### Prerequisites

- Docker installed on your machine.
- Node.js installed (for development purposes).
- PostgreSQL installed locally or use the included Docker setup.

#### Setup Instructions

1. **Clone the Repository**
```bash
   git clone https://github.com/your-username/fullstack-web-app.git
   cd fullstack-web-app
```
2. **Start the Application with Docker Compose** To run all services (frontend, backend, and database) together:
```bash
docker-compose up --build
```
3. **Access the Application**

- **Frontend**: [http://localhost:3000](http://localhost:3000)
- **Backend**: [http://localhost:5000/api](http://localhost:5000/api)
- **PostgreSQL**: Connect to the database on `localhost:5432`

### CI/CD Pipeline

This project integrates CI/CD pipelines using **GitHub Actions**. The pipeline will:

- Run tests
- Lint the code
- Build the Docker images
- Deploy the application (if set up for deployment)

You can find the pipeline configuration in `.github/workflows/ci-cd.yml`.

### License

This project is licensed under the MIT License - see the [LICENSE](./LICENSE) file for details.



