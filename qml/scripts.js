function parseSignInSignUpErrorMessage(msg) {
    if (msg === "INVALID_EMAIL" || msg === "EMAIL_NOT_FOUND"){
        return "Email oyo ezali kosimba te"
    }
    else if (msg === "EMAIL_EXISTS"){
        return "Mutu deci aponi email oyo"
    }
    else if (msg === "MISSING_EMAIL"){
        return "Okomaki email te"
    }
    else if (msg === "MISSING_PASSWORD"){
        return "Okomaki mot de passe te"
    }
    else if (msg === "INVALID_PASSWORD"){
        return "Mot de passe oyo esimbi te"
    }
}
