# Use an official Node.js image
FROM node:16

# Create app directory
WORKDIR /usr/src/app

# Install app dependencies
COPY package.json ./
RUN npm install cors

# Bundle app source
COPY . .

# Expose the port that your app runs on
EXPOSE 5000

# Start the app
CMD [ "node", "server.js" ]
