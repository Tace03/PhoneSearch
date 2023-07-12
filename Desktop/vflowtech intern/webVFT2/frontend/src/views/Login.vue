<template>
    <section class="section section-shaped section-lg my-0 w-100 h-100 d-inline-block">
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
                          header-classes="pb-5"
                          body-classes="bg-white px-lg-5 py-lg-5"
                          class="border-0">
                            <div class="text-center text-muted mb-4">
                                <img class="navbar-brand" width="170" height="80" @click="redir" src="../assets/vftlogo.png">
                            </div>
                            <form class="border border-dark rounded" @submit.prevent="handleLogin" role="form">
                                <base-input
                                            class="mb-3"
                                            placeholder="Email"
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
                                                    v-model="password"
                                                    class="form-control"
                                                    required>
                                            </template>
                                </base-input>
                                <div class="text-danger invalid-feedback" style="display: block;" :class="{'mt-2': hasIcon}" v-if="message">
                                            {{ message }}
                                </div>
                                <base-checkbox>
                                    Remember me
                                </base-checkbox>
                                <div class="text-center">
                                    <base-button type="submit" class="my-4">Sign In</base-button>
                                </div>
                            </form>
                    </card>
                    <div class="row mt-3">
                        <div class="col-6">
                            <p href="#" class="text-light">
                                <small>Forgot password?</small>
                            </p>
                        </div>
                        <div class="col-6 text-right">
                            <p @click="handleSignup" class="text-light">
                                <small>Create new account</small>
                            </p>
                        </div>
                    </div>
                </div>
            </div>
        </div>
    </section>
</template>

<!-- <template>
    <h1>Dashboard Login</h1>
    <form @submit.prevent="handleLogin">
        <label>Enter email: </label>
        <input type="email" v-model="email" required>
        <label>Enter password: </label>
        <input type="password" v-model="password" required>
        <div ref="messagebox" v-if="message">{{ message }}</div>
        <div class="submit">
            <button>Login</button>
        </div>
        <div class="sign-up">
            <button @click="handleSignup">Don't have an account yet?</button>
        </div>
    </form>
</template> -->

<script>
export default {
  name: 'Login',
  data() {
    return {
        email: "",
        password: "",
        message: "" 
    }
  },
  computed: {
    loggedIn() {
      return this.$store.state.auth.status.loggedIn;
    },
  },
  created() {
    if (this.loggedIn) {
      this.$router.push("/orgs");
    }
  },
  methods: {
    handleLogin() {
      this.$store.dispatch("auth/login", {"email": this.email, "password": this.password}).then(
        () => {
          this.$router.push("/orgs");
        },
        (error) => {
            // this.$refs.messagebox.classList.add("error");
            this.message = "Incorrect Password. Please try again.";
        }
      )
    },
    handleSignup() {
        this.$router.push("/sign-up");
    }
  }
}

</script>

<style>
    h1 {
        font-size: 1.5em;
        letter-spacing: 1.5px;
        font-weight: bold;
        text-transform: uppercase;
    }
    form {
        max-width: 420px;
        margin: 30px auto;
        background: white;
        text-align: left;
        padding: 40px;
        border-radius: 10px;
    }
    label {
        color: #aaa;
        display: inline-block;
        margin: 25px 0 15px;
        font-size: 0.5em;
        text-transform: uppercase;
        letter-spacing: 1px;
        font-weight: bold;
    }
    input {
        display: inline-block;
        padding: 10px 6px;
        width: 100%;
        box-sizing: border-box;
        border: none;
        border-bottom: 1px solid #ddd;
        color: #555
    }
    button {
        background: #0b6dff;
        border: 0;
        padding: 10px 20px;
        margin-top: 20px;
        color: white;
        border-radius: 20px;
    }
    .submit, .sign-up {
        text-align: center;
    }
    .error {
        color: #ff0062;
        margin-top: 10px;
        font-size: 0.8em;
        font-weight: bold;
    }
</style>