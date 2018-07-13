/*
login.js: CrunchNet login window script
Copyright (c) 2018 Kade Burnside
*/

/*
This file is part of CrunchNet.

CrunchNet is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

CrunchNet is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with CrunchNet. If not, see <http://www.gnu.org/licenses/>.
*/

// Base sign in function (To be replaced with CRNAPI function)
function BaseSignIn(username, password) {
    window.location = 'main.html';
}

// Sign in function (SignIn.html)
function SignIn_SignIn() {
    // Calls base sign in function
    BaseSignIn(document.getElementById('username').value, document.getElementById('password').value);
}

// Sign up function (SignUp.html)
function SignUp_SignUp() {
    // Temporary function defenition. To be replaced when CRNAPI-JS exists
    BaseSignIn(null, null);
}

// Sign in function (AccountSelect.html)
function AccountSelect_SignIn() {
    // Temporary function defenition. To be replaced when CRNAPI-JS exists
    BaseSignIn(null, null);
}

// Add account function (AccountSelect.html)
function AccountSelect_AddAccount() {
    // Opens sign in page
    window.location = 'SignIn.html';
}

// Remove account function (AccountSelect.html)
function AccountSelect_RemoveAccount() {
    var list = document.getElementById('accountSelect'); // Gets select object
    // Removes currently selected option from list
    list.remove(list.selectedIndex);
}

// Submit password function (PasswordConfirm.html)
function PasswordConfirm_SubmitPassword() {
    // Temporary function defenition. To be replaced when CRNAPI-JS exists
    BaseSignIn(null, null);
}
