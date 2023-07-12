import axios from 'axios';

const API_URL = 'https://backend.vflowtechiot.com/cockpit/api/v1/';

class AuthService {
  async login(user) {
    return axios
      .post(API_URL + 'sign-in', {
        "email": user.email,
        "password": user.password
      })
      .then(response => {
        if (response.data.access_token) {
          sessionStorage.setItem('user', JSON.stringify(response.data));
          sessionStorage.setItem('auth_token', response.data.access_token);
        }
        return response.data;
      });
  }

  signup(user) {
    return axios.post(API_URL + 'sign-up', {
        "email": user.email,
        "password": user.password,
        "first_name": user.first_name
    })
  }

  logout() {
    sessionStorage.removeItem('user');
    sessionStorage.removeItem('auth_token');
  }
}

export default new AuthService();