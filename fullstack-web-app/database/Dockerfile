# Use an official Postgres image
FROM postgres:13

# Add custom initialization SQL scripts
COPY init.sql /docker-entrypoint-initdb.d/
