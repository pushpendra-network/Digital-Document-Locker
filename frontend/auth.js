const AUTH_BASE_URL = "http://localhost:18080";

async function registerUser() {
    const name = document.getElementById("name").value.trim();
    const email = document.getElementById("email").value.trim();
    const password = document.getElementById("password").value.trim();
    const confirmPassword = document.getElementById("confirmPassword").value.trim();

    if (!name || !email || !password || !confirmPassword) {
        alert("Please fill all fields.");
        return;
    }

    if (password !== confirmPassword) {
        alert("Passwords do not match.");
        return;
    }

    try {
        const response = await fetch(AUTH_BASE_URL + "/register", {
            method: "POST",
            headers: {
                "Content-Type": "application/json"
            },
            body: JSON.stringify({
                name: name,
                email: email,
                password: password
            })
        });

        const message = await response.text();

        if (response.ok) {
            alert(message);
            window.location.href = "login.html";
        } else {
            alert(message);
        }
    } catch (error) {
        alert("Cannot connect to server.");
    }
}

async function loginUser() {
    const email = document.getElementById("email").value.trim();
    const password = document.getElementById("password").value.trim();

    if (!email || !password) {
        alert("Please enter email and password.");
        return;
    }

    try {
        const response = await fetch(AUTH_BASE_URL + "/login", {
            method: "POST",
            headers: {
                "Content-Type": "application/json"
            },
            body: JSON.stringify({
                email: email,
                password: password
            })
        });

        const message = await response.text();

        if (response.ok) {
            alert(message);
            window.location.href = "index.html";
        } else {
            alert(message);
        }
    } catch (error) {
        alert("Cannot connect to server.");
    }
}