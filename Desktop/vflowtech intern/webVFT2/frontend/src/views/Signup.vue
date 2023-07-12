<template>
    <section class="section section-shaped section-lg my-0">
        <div class="shape shape-style-1 bg-gradient-default">
            <span></span>
            <span></span>
            <span></span>
            <span></span>
            <span></span>
            <span></span>
            <span></span>
            <span></span>
        </div>
        <div class="container pt-lg-md">
            <div class="row justify-content-center">
                <div class="col-lg-5">
                    <card type="secondary" shadow
                          header-classes="bg-white pb-5"
                          body-classes="px-lg-5 py-lg-5"
                          class="border-0">
                            <div class="text-center text-muted mb-4">
                                <small>Sign up</small>
                            </div>
                            <form @submit.prevent="signUp" role="form">
                                <base-input
                                            class="mb-3"
                                            addon-left-icon="ni ni-email-83">
                                            <template #inputbox>
                                                <input
                                                    type="text"
                                                    placeholder="First name"
                                                    v-model="name"
                                                    class="form-control"
                                                    required>
                                            </template>
                                </base-input>
                                <base-input
                                            class="mb-3"
                                            addon-left-icon="ni ni-email-83">
                                            <template #inputbox>
                                                <input
                                                    type="email"
                                                    placeholder="Email"
                                                    v-model="email"
                                                    class="form-control"
                                                    required>
                                            </template>
                                </base-input>
                                <base-input
                                            type="password"
                                            placeholder="Password"
                                            addon-left-icon="ni ni-lock-circle-open">
                                            <template #inputbox>
                                                <input
                                                    type="password"
                                                    placeholder="Password"
                                                    @keyup="verifyPassword"
                                                    v-model="password"
                                                    class="form-control"
                                                    required>
                                            </template>
                                </base-input>
                                <div class="text-danger invalid-feedback" style="display: block;" :class="{'mt-2': hasIcon}" v-if="passwordError">
                                        {{ passwordError }}
                                </div>
                                <base-input
                                            type="password"
                                            placeholder="Repeat password"
                                            addon-left-icon="ni ni-lock-circle-open">
                                            <template #inputbox>
                                                <input
                                                    type="password"
                                                    placeholder="Repeat password"
                                                    @keyup="verifyPassword2"
                                                    v-model="verify"
                                                    class="form-control"
                                                    required>
                                            </template>
                                </base-input>
                                <div class="text-danger invalid-feedback" style="display: block;" :class="{'mt-2': hasIcon}" v-if="passwordError2">
                                        {{ passwordError2 }}
                                </div>

                                <div class="text-danger invalid-feedback" style="display: block;" :class="{'mt-2': hasIcon}" v-if="message">
                                            {{ message }}
                                </div>
                                <div class="text-center">
                                    <base-button type="submit" class="my-4">Sign Up</base-button>
                                </div>
                            </form>
                    </card>
                    <div class="row mt-3">
                        <div class="col-6 text-right">
                            <p @click="handleSignin" class="text-light">
                                <small>Have an account?</small>
                            </p>
                        </div>
                    </div>
                </div>
            </div>
        </div>
    </section>
</template>


<!-- <template>
    <h1>Dashboard Sign-up</h1>
    <form @submit.prevent="signUp">
        <label>First Name: </label>
        <input type="text" v-model="name" required>
        <label>Enter email: </label>
        <input type="email" v-model="email" required>
        <label>Enter password: </label>
        <input @keyup="verifyPassword" type="password" v-model="password" required>
        <div v-if="passwordError" class="error">{{ passwordError }}</div>
        <label>Repeat password: </label>
        <input @keyup="verifyPassword2" type="password" v-model="verify" required>
        <div v-if="passwordError2" class="error">{{ passwordError2 }}</div>
        <div ref="messagebox" v-if="message">{{ message }}</div>
        <div class="submit">
            <button>Sign up</button>
        </div>
        <div class="sign-up">
            <button @click="handleSignin">Have an account?</button>
        </div>
    </form>
</template> -->

<script>
export default {
  name: 'Signup',
  data() {
    return {
        email: "",
        password: "",
        passwordError: "",
        passwordError2: "",
        verify: "",
        name: "",
        message: "" 
        }
    },
    computed: {
        loggedIn() {
            return this.$store.state.auth.status.loggedIn;
        }
    },
    created() {
        if (this.loggedIn) {
        this.$router.push("/orgs");
        }
    },
  methods: {
    verifyPassword() {
        function verifyInput(input) {
            const uppercaseRegex = /[A-Z]/;
            const lowercaseRegex = /[a-z]/;
            const specialCharRegex = /[@#$!%^*?&()_|><+:;,/.*]/;
            return (
                uppercaseRegex.test(input) &&
                lowercaseRegex.test(input) &&
                specialCharRegex.test(input)
            );
        }
        if ((this.password.length < 8) || !verifyInput(this.password)) {
            this.passwordError = "Minimum eight characters, at least one uppercase letter, one lowercase letter, one number and one special character (@#$!%^*?&()_|><+:;,/.*)"
        } else {
            this.passwordError = ''
        }
    },
    verifyPassword2() {
        if (this.password !== this.verify) {
            this.passwordError2 = "Password and confirm password must be same"
        } else {
            this.passwordError2 = ''
        }
    },
    signUp() {
        this.$store.dispatch("auth/signup", {"email": this.email, "password": this.password, "first_name": this.name})
            .then(() => {
                this.$refs.messagebox.classList.add("success");
                this.message = "Sign up successful. You will receive a confirmation email when the admin approves.";  
            })
            .catch((error) => {
                this.$refs.messagebox.classList.add("error");
                this.message = "Sign up failed. Please try again.";
            })
        
    },
    handleSignin() {
        this.$router.push("/login");
    }
  }
}

</script>

<style>
.success {
        color: #70c927;
        margin-top: 10px;
        font-size: 0.8em;
        font-weight: bold;
    }
</style>