//
// Created by mustafa on 8/12/24.
//

#ifndef EMBEDDED_FILES_H
#define EMBEDDED_FILES_H

const char* index_html = R"(

<!DOCTYPE html>
<html lang="tr">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Telefon Rehberi</title>
    <link rel="stylesheet" href="style.css">
    <style>

*:not(input):not(textarea) {
    -webkit-user-select: none;
    -ms-user-select: none;
    user-select: none
}
body {
    background-image: url("backgroundapp.png");
    color: #f4f4f4;
    font-family: 'Garamond', serif;
    overflow: hidden;
    display: flex;
    justify-content: center;
    align-items: center;
    height: 100vh;
    margin: 0;
    cursor: url('data:image/svg+xml;utf8,<svg xmlns="http://www.w3.org/2000/svg" width="16" height="16" viewBox="0 0 16 16"><polygon points="0,0 16,8 0,16" fill="green"/></svg>'), auto !important;

}

::-webkit-scrollbar {
    size: 0px;
}


/* Yeşil Cursor */

.green-cursor {
    cursor: url('data:image/svg+xml;utf8,<svg xmlns="http://www.w3.org/2000/svg" width="24" height="24" viewBox="0 0 24 24" fill="none" stroke="green" stroke-width="2"><circle cx="12" cy="12" r="10"/></svg>'), auto;
}

.green-cursor * {
    cursor: inherit !important;
}

.contact-form {
    background: #333;
    border-radius: 20px;
    box-shadow: 0 15px 45px rgba(0, 0, 0, 0.8);
    padding: 20px;
    width: 90%;
    max-width: 1920px;
    height: 80%;
    max-height: 800px;
    overflow-y: auto;
    position: relative;
    opacity: 80%;
}

h1 {
    font-size: 2.5em;
    text-align: center;
    margin-bottom: 20px;
    color: #d1b179;
    text-shadow: 2px 2px 5px rgba(0, 0, 0, 0.5);
    border-bottom: 2px solid #d1b179;
    padding-bottom: 10px;
}

table {
    width: 100%;
    border-collapse: collapse;
    margin-top: 20px;
    font-size: 1.5em;
}

th, td {
    text-align: left;
    padding: 15px;
    border: 1px solid #444;
    color: #f4f4f4;
}

th {
    background-color: #555;
    color: #d1b179;
    font-size: 1.2em;
    text-shadow: 1px 1px 3px rgba(0, 0, 0, 0.3);
}

tr:nth-child(even) {
    background-color: #131313;
}

button {
    background: linear-gradient(145deg, #2a2a2a, #1c1c1c);
    border: none;
    border-radius: 10px;
    color: #d1b179;
    padding: 15px 25px;
    font-size: 1.2em;
    cursor: pointer;
    box-shadow: 0 4px 8px rgba(0, 0, 0, 0.5);
    transition: transform 0.3s ease, background 0.3s ease;
}

button:hover {
    background: linear-gradient(145deg, #3a3a3a, #2a2a2a);
    transform: scale(1.05);
}

button:active {
    transform: scale(0.95);
}

/* Existing styles... */

.popup {
    display: none;
    position: fixed;
    top: 0;
    left: 0;
    width: 100%;
    height: 100%;
    background: rgba(0, 0, 0, 0.8);
    justify-content: center;
    align-items: center;
    opacity: 90%;
}

.popup-content {
    background: #222;
    padding: 30px;
    border-radius: 15px;
    box-shadow: 0 15px 30px rgba(0, 0, 0, 0.8);
    width: 80%;
    max-width: 600px;
    animation: fadeIn 0.5s ease-in-out;
    position: relative;
}

@keyframes fadeIn {
    from {
        opacity: 0;
        transform: translateY(-30px);
    }
    to {
        opacity: 1;
        transform: translateY(0);
    }
}

.popup-content h2 {
    font-size: 1.8em;
    margin-bottom: 20px;
    color: #d1b179;
    text-shadow: 0 0 10px rgba(255, 215, 0, 0.7);
}

.popup-content label {
    display: block;
    margin-bottom: 5px;
    color: #f4f4f4;
    font-size: 1.1em;
}

.popup-content input {
    width: calc(100% - 20px);
    padding: 10px;
    margin-bottom: 15px;
    border: 1px solid #444;
    border-radius: 10px;
    background: #333;
    color: #f4f4f4;
    font-size: 1em;
}

.popup-content .button-container {
    display: flex;
    justify-content: flex-end;
    gap: 15px;
}

.popup-content button {
    padding: 12px 20px;
    font-size: 1em;
    background-color: #555;
    color: #d1b179;
    border: none;
    border-radius: 10px;
}

#cancelBtn {
    background-color: #aa0000;
}

#cancelBtn:hover {
    background-color: #880000;
}

#saveBtn {
    background-color: #006600;
}

#saveBtn:hover {
    background-color: #004d00;
}

.closeBtn {
    position: absolute;
    top: 10px;
    right: 10px;
    font-size: 24px;
    color: #d1b179;
    cursor: pointer;
}

    </style>

</head>
<body>
<div class="contact-form" id="contact-form">
    <h1>Telefon Rehberi</h1>
    <button id="addContactBtn">Ekle</button>
    <table>
        <thead>
            <tr>
                <th>Ad</th>
                <th>Soyad</th>
                <th>Telefon</th>
                <th>Sil</th>
            </tr>
        </thead>
        <tbody id="contact-list">
            <!-- Kişiler burada görünecek -->
        </tbody>
    </table>
</div>

<div class="popup" id="popup">
    <div class="popup-content">
        <span class="closeBtn" id="closePopup">&times;</span>
        <h2>Yeni Kişi Ekle</h2>
        <label for="popup-first-name">Ad</label>
        <input type="text" id="popup-first-name" placeholder="Ad" required>
        <label for="popup-last-name">Soyad</label>
        <input type="text" id="popup-last-name" placeholder="Soyad" required>
        <label for="popup-phone">Telefon</label>
        <input type="text" id="popup-phone" placeholder="Telefon" required>
        <div class="button-container">
            <button id="cancelBtn">İptal</button>
            <button id="saveBtn">Kaydet</button>
        </div>
    </div>
</div>

<script src="script.js"></script>
</body>

<script>document.addEventListener('DOMContentLoaded', () => {
    const addContactBtn = document.getElementById('addContactBtn');
    const popup = document.getElementById('popup');
    const closePopup = document.getElementById('closePopup');
    const cancelBtn = document.getElementById('cancelBtn');
    const saveBtn = document.getElementById('saveBtn');

    addContactBtn.addEventListener('click', () => {
        popup.style.display = 'flex';
    });

    closePopup.addEventListener('click', () => {
        popup.style.display = 'none';
    });

    cancelBtn.addEventListener('click', () => {
        popup.style.display = 'none';
    });

    saveBtn.addEventListener('click', () => {
        const firstName = document.getElementById('popup-first-name').value;
        const lastName = document.getElementById('popup-last-name').value;
        const phone = document.getElementById('popup-phone').value;

        if (firstName && lastName && phone) {
            addContactToTable(firstName, lastName, phone);
            saveContact(firstName, lastName, phone);

            popup.style.display = 'none';

            document.getElementById('popup-first-name').value = '';
            document.getElementById('popup-last-name').value = '';
            document.getElementById('popup-phone').value = '';
        }
    });

    function addContactToTable(firstName, lastName, phone) {
        const contactList = document.getElementById('contact-list');
        const newRow = document.createElement('tr');

        newRow.innerHTML = `
            <td>${firstName}</td>
            <td>${lastName}</td>
            <td>${phone}</td>
            <td><button class="deleteBtn">Sil</button></td>
        `;

        contactList.appendChild(newRow);

        newRow.querySelector('.deleteBtn').addEventListener('click', () => {
            deleteContact(firstName, lastName, phone);
            newRow.remove();
        });
    }

    function saveContact(firstName, lastName, phone) {
        webview.call('add_contact', JSON.stringify({ ad: firstName, soyad: lastName, telefon: phone }));
    }

    function deleteContact(firstName, lastName, phone) {
        webview.call('delete_contact', JSON.stringify({ ad: firstName, soyad: lastName, telefon: phone }));
    }

    function loadContacts() {
        webview.call('get_contacts', '', function(response) {
            const contacts = JSON.parse(response);
            contacts.forEach(contact => {
                addContactToTable(contact.ad, contact.soyad, contact.telefon);
            });
        });
    }

    loadContacts();
});
</script>

</html>


)";


#endif //EMBEDDED_FILES_H
