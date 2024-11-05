// Get references to HTML elements
const uploadImageInput = document.getElementById('uploadImageInput');
const uploadButton = document.getElementById('uploadButton');
const uploadStatus = document.getElementById('uploadStatus');

const searchImageInput = document.getElementById('searchImageInput');
const searchButton = document.getElementById('searchButton');
const numResultsInput = document.getElementById('numResults');

const searchStatus = document.getElementById('searchStatus');
const resultGallery = document.getElementById('resultGallery'); 

// Function to display search results in the gallery
function displayResults(images) {
   
    resultGallery.innerHTML = ''; // Clear any existing images

    numbers.forEach(imagePath => {
        const img = document.createElement('img');
        img.src = `/Images/${imagePath}`;
        img.alt = imagePath;
        img.style.maxWidth = '150px'; // Adjust size as needed
        img.style.margin = '5px';
        resultGallery.appendChild(img);
    });
}
function uploadImage(file, isSearch) {
    const formData = new FormData();
    formData.append('image', file); // Attach the image file

    // Check if the operation is a search
    if (isSearch) {
        const number = numResultsInput.value;
        if (!number) {
            alert('Please enter a number for the search results.');
            return;
        }
        formData.append('number', number); // Add number of results to formData
        searchStatus.textContent = 'Searching for similar images...';
    } else {
        uploadStatus.textContent = 'Uploading image...';
    }

    const endpoint = isSearch ? "http://localhost:8000/search" : "http://localhost:8000/upload";

    // Make a POST request to the server with form data
    fetch(endpoint, {
        method: 'POST',
        body: formData // Send the FormData instead of JSON
    })
    .then(response => {
        if (!response.ok) {
            throw new Error('Network response was not ok');
        }
        return response.json();
    })
    .then(data => {
        console.log('JSON data', data);
      
    if (isSearch) {
        
        searchStatus.textContent = data.message
        if ( data.numbers.length > 0) {
            displayResults(data.numbers); // Pass the array to displayResults
        } else {
            searchStatus.textContent = 'No results ';
        }
    } else {
        
        uploadStatus.textContent = data.message;
    }
    })
    .catch(error => {
        console.error('Error during image processing:', error);
        if (isSearch) {
            searchStatus.textContent = 'Error during image search: ' + error.message;
        } else {
            uploadStatus.textContent = 'Error during image upload: ' + error.message;
        }
    });
}


// Handle image upload for storage
uploadButton.addEventListener('click', () => {
    if (uploadImageInput.files.length === 0) {
        alert('Please select an image to upload.'); // Alert if no file selected
        return;
    }
    const file = uploadImageInput.files[0]; // Get the selected file
    uploadImage(file, false); // Call uploadImage for upload operation
});

// Handle image search
searchButton.addEventListener('click', () => {
    if (searchImageInput.files.length === 0) {
        alert('Please select an image for searching.'); // Alert if no file selected
        return;
    }
    const file = searchImageInput.files[0]; // Get the selected file
    uploadImage(file, true); // Call uploadImage for search operation
});
