const express = require('express');
const multer = require('multer');
const path = require('path');
const fs = require('fs');
const cors = require('cors');
const { exec } = require('child_process');
const app = express();

// Define the upload directory and JSON file path
const uploadDir = path.join(__dirname, 'Images');
const jsonFilePath = path.join(__dirname, 'input.json');

// Ensure the images directory exists
if (!fs.existsSync(uploadDir)) {
    fs.mkdirSync(uploadDir, { recursive: true });
}

// Set up multer storage
const storage = multer.diskStorage({
    destination: (req, file, cb) => {
        cb(null, uploadDir);
    },
    filename: (req, file, cb) => {
        cb(null, file.originalname);
    }
});

const upload = multer({ storage: storage });

app.use(cors());
app.use(express.json());
app.use(express.static('public'));

// Utility function to execute a command with a promise
function runCommand(command) {
    return new Promise((resolve, reject) => {
        exec(command, (error, stdout, stderr) => {
            if (error) {
                console.error(`Error executing ${command}: ${error.message}`);
                console.error(`stderr: ${stderr}`);
                reject(error);
                return;
            }
            console.log(`Command output: ${stdout}`);
            resolve(stdout);
        });
    });
}

// Function to write JSON data to file
function writeJsonFile(filePath, data) {
    return new Promise((resolve, reject) => {
        fs.writeFile(filePath, JSON.stringify(data, null, 2), (err) => {
            if (err) {
                console.error(`Error writing to ${filePath}: ${err}`);
                reject(err);
            } else {
                console.log(`Successfully wrote data to ${filePath}`);
                resolve();
            }
        });
    });
}

// Endpoint for handling image uploads and search

app.post('/upload', upload.single('image'), async (req, res) => {
    });
app.post('/search', upload.single('image'), async (req, res) => {
    const { number } = req.body;

    if (!req.file || !number) {
        return res.status(400).json({ success: false, message: 'File and number are required' });
    }

    const uploadedFileName = req.file.originalname;
    const data = { target: uploadedFileName, num: parseInt(number) };

    try {
        // Step 1: Write to input.json
        await writeJsonFile(jsonFilePath, data);

        // Step 2: Run histogram.exe and wait for completion
        const histogramExePath = path.join(__dirname, 'histogram.exe');
        await runCommand(`"${histogramExePath}" "${path.join(uploadDir, uploadedFileName)}"`);

        // Step 3: Run project.exe after histogram.exe completes
        const projectExePath = path.join(__dirname, 'project.exe');
        await runCommand(`"${projectExePath}"`);

        // Step 4: Read the updated data from input.json and send to client
        const jsonData = JSON.parse(fs.readFileSync(jsonFilePath, 'utf8'));
    res.status(200).json(jsonData);
   
      
    } catch (error) {
        res.status(500).json({ success: false, message: 'An error occurred during processing.' });
    }
});

// Start the server
const PORT = 8000;
app.listen(PORT, () => {
    console.log(`Server is running on http://localhost:${PORT}`);
});
