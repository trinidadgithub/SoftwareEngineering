const express = require('express');
const { Pool } = require('pg');
const app = express();
const port = 5000;
const cors = require('cors');

// Enable CORS for all requests coming from the frontend (http://localhost:3000)
app.use(cors({
  origin: 'http://localhost:3000',
}));

// PostgreSQL connection setup
const pool = new Pool({
  user: 'postgres',
  host: 'database',
  database: 'mydb',
  password: 'postgres',
  port: 5432,
});

app.get('/', (req, res) => {
  res.send('Welcome to the backend API!');
});

app.get('/api/data', async (req, res) => {
  try {
    const result = await pool.query('SELECT * FROM users');
    res.json(result.rows);
  } catch (err) {
    console.error(err.message);
    res.status(500).send('Server Error');
  }
});

app.listen(port, () => {
  console.log(`Server is running on port ${port}`);
});
