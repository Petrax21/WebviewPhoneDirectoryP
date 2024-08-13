document.addEventListener('DOMContentLoaded', () => {
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
