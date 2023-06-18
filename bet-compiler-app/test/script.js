const fileList = document.getElementById('fileList');

// Simulated array of file names
const files = ['file1.txt', 'file2.txt', 'file3.txt'];

// Generate file list items
files.forEach((file) => {
  const listItem = document.createElement('li');
  listItem.textContent = file;
  fileList.appendChild(listItem);
});

