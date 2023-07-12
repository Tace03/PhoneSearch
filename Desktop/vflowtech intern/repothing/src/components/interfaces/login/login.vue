<script>
import * as Sentry from "@sentry/vue";

export default {
    name: "LoginFunctions",
    data() {
        return {
            url: "https://backend.vflowtechiot.com",
            error: "",
            email: "",
            password: "",
        }
    },
    mounted() {
        if(localStorage.getItem("authToken") !== null)
            this.$router.push({ name: 'queryData' })
    },
    methods: {
        LoginUserKeyPressed(event) {
            if (event.key === "Enter")
                this.LoginUser();
        },
        LoginUser() {
            this.error = "";
            let query = this.url.concat("/api/sign-in");
            Sentry.captureMessage("Query Made - User Sign In", { extra: { query: query } });
            fetch(query, {
                "method": "POST",
                "headers": { 'Content-Type': 'application/json' },
                "body": JSON.stringify({
                    email: this.email.trim(),
                    password: this.password
                })
            })
            .then(response => response.json())
            .then(response => {
                if ("detail" in response)
                {
                    if (response["detail"] === "Credentials are incorrect.")
                    {
                        this.error = "Invalid User"
                        Sentry.captureMessage("Query Made - User Sign In Error",
                        {
                            extra: {
                                query: query,
                                "Attempt Account": this.email.trim()
                            }
                        });
                    }
                    else if (response["detail"] === "Parameters provided to perform current action is either not valid or missing or not unique")
                    {
                        this.error = "Please enter all parameters"
                    }
                    else
                    {
                        this.error = "VFTIoT Server Error."
                        Sentry.captureMessage("User Sign In API Error Message", { extra: response });
                    }
                
                }
                else {
                    query = this.url.concat("/api/orgs/3/sign-in")
                    fetch(query, {
                        "method": "POST",
                        "headers": {
                            'Content-Type': 'application/json',
                            'Auth-Token': response.access_token
                        },
                        "body": JSON.stringify({})
                    })
                    .then(response => response.json())
                    .then(response => {
                        if ("detail" in response)
                        {
                            this.error = "VFTIoT Server Error."
                            Sentry.captureMessage("Organisation Sign In API Error Message", { extra: response })
                        }
                        else
                        {
                            localStorage.setItem("loggedUser", this.email.trim());
                            localStorage.setItem("authToken", response.access_token);
                            this.$router.push({ name: 'queryData' })
                        }
                    })
                    .catch(err => {
                        this.error = "Unknown error, error Recorded."
                        Sentry.captureMessage("(Fetch Function) Organisation Sign In Error: " + err)
                    });
                }
            })
            .catch(err => {
                this.error = "Unknown error, error Recorded."
                Sentry.captureMessage("(Fetch Function) User Sign In: " + err)
            });
        },
    }
}
</script>

<template>
    <div class="LoginContainer">
        <div class="LoginMainContainer">
            <div class="LoginWebLogoContainer">
                <img class="LoginWebLogo" src="@/assets/vftLogo.svg" />
            </div>
            <div class="LoginInputContainer">
                <div class="InputContainer">
                    <div class="InputName">Email:</div>
                    <input class="InputValue" @keydown="LoginUserKeyPressed" v-model="email" />
                </div>
                <div class="InputContainer">
                    <div class="InputName">Password:</div>
                    <input class="InputValue" type="password" @keydown="LoginUserKeyPressed" v-model="password" />
                </div>
                <div class="ErrorMsg" v-if="error != ''">{{ error }}</div>
                <button class="LoginButton" @click="LoginUser">Login</button>
            </div>
        </div>
    </div>
</template>

<style>
.LoginContainer {
    display: flex;
    min-height: 91.8vh;
    align-items: center;
    justify-content: center;
}

.LoginMainContainer {
    display: flex;
    border: solid;
    padding: 3rem 1rem;
    border-radius: 1rem;
    border-width: 0.1rem;
    flex-direction: column;
    background-color: #FAF9F9;
    box-shadow: rgba(0, 0, 0, 0.25) 0px 54px 55px, rgba(0, 0, 0, 0.12) 0px -12px 30px, rgba(0, 0, 0, 0.12) 0px 4px 6px, rgba(0, 0, 0, 0.17) 0px 12px 13px, rgba(0, 0, 0, 0.09) 0px -3px 5px;

}

.LoginWebLogoContainer {
    width: 100%;
    display: flex;
    justify-content: center;
}

.LoginWebLogo {
    max-width: 10rem;
    max-height: 10rem;
}

.LoginInputContainer {
    gap: 0.5rem;
    display: flex;
    border: solid;
    padding: 1rem;
    margin-top: -1.5rem;
    border-width: 0.1rem;
    flex-direction: column;
}

.LoginButton {
    font-size: 1.2rem;
    margin-top: 1rem;
}

.InputValue {
    font-size: 1.1rem;
    width: 15rem;
}

.InputName {
    font-size: 1.2rem;
}

.ErrorMsg{
    font-size: 1.1rem;
    text-align: center;
    color: rgb(221, 10, 10);
}
</style>