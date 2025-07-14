const express = require('express');
const fs = require('fs');
const { exec } = require('child_process');

const app = express();
app.use(express.static('.'));
app.use(express.json());

app.post('/api/analyze', (req, res) => {
  // Write input.json
  fs.writeFileSync('input.json', JSON.stringify(req.body, null, 2));

  // Execute C++ program
  exec('./analyzer', (err, stdout, stderr) => {
    if (err) {
      console.error('Execution error:', err);
      res.status(500).send('C++ execution failed');
      return;
    }

    // Read output.json and send its contents
    try {
      const outputData = fs.readFileSync('output.json', 'utf-8');
      const parsed = JSON.parse(outputData);
      res.json(parsed);  // ✅ send to frontend
    } catch (readErr) {
      console.error('Error reading output.json:', readErr);
      res.status(500).send('Failed to read output.json');
    }
  });
});

app.listen(3000, () => {
  console.log('Server running at http://localhost:3000');
});
