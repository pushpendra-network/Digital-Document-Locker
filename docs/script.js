const BASE_URL = "https://digital-document-locker.onrender.com";

const contentBox = document.getElementById("content");
const previewBox = document.getElementById("preview");
const statusBox = document.getElementById("status");

/*==============================
    LIVE PREVIEW
==============================*/

if (contentBox && previewBox) {
    contentBox.addEventListener("input", () => {
        const text = contentBox.value.trim();

        previewBox.innerText =
            text || "Your document preview will appear here...";
    });
}

/*==============================
    STATUS BAR
==============================*/

function setStatus(message, type = "success") {

    if (!statusBox) return;

    const styles = {

        success: {
            color: "#00e096",
            icon: "fa-circle-check"
        },

        error: {
            color: "#ff4d5e",
            icon: "fa-circle-xmark"
        },

        warning: {
            color: "#ffb020",
            icon: "fa-triangle-exclamation"
        },

        info: {
            color: "#38bdf8",
            icon: "fa-circle-info"
        }

    };

    const current = styles[type] || styles.success;

    statusBox.innerHTML = `
        <i class="fa-solid ${current.icon}"></i>
        <span>${message}</span>
    `;

    statusBox.style.color = current.color;
}

/*==============================
    GET DOCUMENT OBJECT
==============================*/

function getDocumentObject() {

    return {

        id: document.getElementById("id").value.trim(),

        title: document.getElementById("title").value.trim(),

        createdAt: document.getElementById("createdAt").value.trim(),

        updatedAt: document.getElementById("updatedAt").value.trim(),

        elements: [

            {

                type: "text",

                content: document.getElementById("content").value

            }

        ]

    };

}

/*==============================
    VALIDATION
==============================*/

function validateDocumentId() {

    const id = document.getElementById("id").value.trim();

    if (id === "") {

        setStatus("Please enter Document ID.", "warning");

        document.getElementById("id").focus();

        return false;

    }

    return true;

}

/*==============================
    LOADING CURSOR
==============================*/

function startLoading() {

    document.body.style.cursor = "wait";

}

function stopLoading() {

    document.body.style.cursor = "default";

}

/*==============================
    SAVE DOCUMENT
==============================*/

async function saveDocument() {

    try {

        if (!validateDocumentId()) return;

        startLoading();

        setStatus("Saving document...", "info");

        const response = await fetch(BASE_URL + "/document", {

            method: "POST",

            headers: {

                "Content-Type": "application/json"

            },

            body: JSON.stringify(getDocumentObject())

        });

        const message = await response.text();

        if (response.ok) {

            setStatus(message, "success");

        }

        else {

            setStatus(message, "error");

        }

    }

    catch (err) {

        setStatus("Cannot connect to server.", "error");

    }

    finally {

        stopLoading();

    }

}

/*==============================
    LOAD DOCUMENT
==============================*/

async function loadDocument() {

    try {

        if (!validateDocumentId()) return;

        startLoading();

        setStatus("Loading document...", "info");

        const id = document.getElementById("id").value.trim();

        const response = await fetch(BASE_URL + "/document/" + id);

        if (!response.ok) {

            setStatus("Document not found.", "warning");

            return;

        }

        const doc = await response.json();

        document.getElementById("title").value =
            doc.title || "";

        document.getElementById("createdAt").value =
            doc.createdAt || "";

        document.getElementById("updatedAt").value =
            doc.updatedAt || "";

        let content = "";

        if (doc.elements && doc.elements.length > 0) {

            content = doc.elements[0].content || "";

        }

        document.getElementById("content").value = content;

        previewBox.innerText =
            content || "Your document preview will appear here...";

        setStatus("Document loaded successfully.", "success");

    }

    catch (err) {

        setStatus("Cannot connect to server.", "error");

    }

    finally {

        stopLoading();

    }

}

/*==============================
    UPDATE DOCUMENT
==============================*/

async function updateDocument() {

    try {

        if (!validateDocumentId()) return;

        startLoading();

        setStatus("Updating document...", "info");

        const id = document.getElementById("id").value.trim();

        const response = await fetch(BASE_URL + "/document/" + id, {

            method: "PUT",

            headers: {

                "Content-Type": "application/json"

            },

            body: JSON.stringify(getDocumentObject())

        });

        const message = await response.text();

        if (response.ok) {

            setStatus(message, "success");

        }

        else {

            setStatus(message, "error");

        }

    }

    catch (err) {

        setStatus("Cannot connect to server.", "error");

    }

    finally {

        stopLoading();

    }

}

/*==============================
    DELETE DOCUMENT
==============================*/

async function deleteDocument() {

    try {

        if (!validateDocumentId()) return;

        const ok = confirm(
            "Are you sure you want to delete this document?"
        );

        if (!ok) {

            setStatus("Deletion cancelled.", "warning");

            return;

        }

        startLoading();

        setStatus("Deleting document...", "info");

        const id = document.getElementById("id").value.trim();

        const response = await fetch(BASE_URL + "/document/" + id, {

            method: "DELETE"

        });

        const message = await response.text();

        if (response.ok) {

            document.getElementById("id").value = "";

            document.getElementById("title").value = "";

            document.getElementById("createdAt").value = "";

            document.getElementById("updatedAt").value = "";

            document.getElementById("content").value = "";

            previewBox.innerText =
                "Your document preview will appear here...";

            setStatus(message, "success");

        }

        else {

            setStatus(message, "error");

        }

    }

    catch (err) {

        setStatus("Cannot connect to server.", "error");

    }

    finally {

        stopLoading();

    }

}

/*==============================
    INITIAL STATUS
==============================*/

setStatus("Digital Document Locker Ready.", "success");