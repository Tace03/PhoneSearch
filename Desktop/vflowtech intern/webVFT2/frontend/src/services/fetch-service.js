import axios from 'axios';

const API_URL = 'https://backend.vflowtechiot.com/cockpit/api/v1/';

class FetchService {
    async fetch_dash_list(id) {
    const query = `org/${id}/get_shared_resources?type=dashboard&page_size=100&page_number=0`
    const TOKEN = sessionStorage.getItem('auth_token');
    const startTime = performance.now();
    return axios
      .get(API_URL + query, {
        headers: {
        'Authorization': `Bearer ${TOKEN}` 
      }})
      .then(response => {
        const endTime = performance.now(); // Record end time
        const duration = endTime - startTime; // Calculate duration
        console.log(`${API_URL + query}\nStart: ${startTime}, End: ${endTime}\nTotal: ${duration} ms.`);
        return response.data;
      });
    }
    // async fetch_dash(org_id, dash_id) {
    //     const query = `org/${org_id}/dashboard/${dash_id}`
    //     const TOKEN = sessionStorage.getItem('auth_token');
    //     const resources = [];
    //     const dash = await axios.get(API_URL + query, {
    //                 headers: {
    //                 'Authorization': `Bearer ${TOKEN}` 
    //                 }})
    //     for (let i = 0; i < dash.data.panels.length; i++) {
    //         const panel_data = await this.fetch_panel(org_id, dash_id, dash.data.panels[i].id);
    //         for (let j = 0; j < panel_data.widgets.length; j++) {
    //             const res = await this.fetch_widget(org_id, dash_id, dash.data.panels[i].id, panel_data.widgets[j].widget_id, panel_data.widgets[j].id).then(
    //                 (widget) => resources.push(widget)
    //             );
    //         }
    //     }
    //     return resources;
    // }
    async fetch_dash(org_id, dash_id) {
        const query = `org/${org_id}/dashboard/${dash_id}`;
        const TOKEN = sessionStorage.getItem('auth_token');
        const startTime = performance.now();
        const dash = await axios.get(API_URL + query, {
          headers: {
            'Authorization': `Bearer ${TOKEN}` 
          }
        });
        const endTime = performance.now(); // Record end time
        const duration = endTime - startTime; // Calculate duration
        console.log(`${API_URL + query}\nStart: ${startTime}, End: ${endTime}\nTotal: ${duration} ms.`);
        return dash.data
    }
    async fetch_panel(org_id, dash_id, panel_id) {
        const query = `org/${org_id}/dashboard/${dash_id}/panels/${panel_id}`
        const TOKEN = sessionStorage.getItem('auth_token');
        const startTime = performance.now();
        const panel_data = await axios
          .get(API_URL + query, {
            headers: {
            'Authorization': `Bearer ${TOKEN}` 
          }})
        const endTime = performance.now(); // Record end time
        const duration = endTime - startTime; // Calculate duration
        console.log(`${API_URL + query}\nStart: ${startTime}, End: ${endTime}\nTotal: ${duration} ms.`);
        const real_time = panel_data.data.widgets.filter(widget => {
            return widget.filter_metadata.type === "realtime"
        });
        const widgetPromises = panel_data.data.widgets.map(widget => {
            return this.fetch_widget(org_id, dash_id, panel_id, widget.widget_id, widget.id);
        });
        const values = await Promise.all(widgetPromises)
        const result = {data: values, realtime: real_time};
        return result;
    }
    async fetch_widget(org_id, dash_id, panel_id, widget_type, widget_id) {
        const query = `org/${org_id}/dashboard/${dash_id}/panels/${panel_id}/widgets/${widget_type}/widget_instances/${widget_id}`
        const TOKEN = sessionStorage.getItem('auth_token');
        const startTime = performance.now();
        return axios
          .get(API_URL + query, {
            // params: params,
            headers: {
            'Authorization': `Bearer ${TOKEN}` 
          }})
          .then(response => {
            const endTime = performance.now(); // Record end time
            const duration = endTime - startTime; // Calculate duration
            console.log(`${API_URL + query}\nStart: ${startTime}, End: ${endTime}\nTotal: ${duration} ms.`);
            return response.data;
          });
    }
}

export default new FetchService();