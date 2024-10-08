# Stage 1: Build the React app
FROM node:16-alpine as build

WORKDIR /usr/src/app

# Copy package.json and package-lock.json files
COPY package.json package-lock.json ./

# Install dependencies
RUN npm install

# Copy the rest of the application code
COPY . .
COPY public /usr/src/app/public

# Build the React app
RUN npm run build

# Stage 2: Serve the React app using an HTTP server
FROM nginx:alpine
COPY --from=build /usr/src/app/build /usr/share/nginx/html

# Expose port 3000 to the outside world
EXPOSE 3000

# Start nginx
CMD ["nginx", "-g", "daemon off;"]
