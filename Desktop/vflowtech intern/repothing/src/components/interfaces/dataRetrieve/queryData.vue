<script>
import * as Sentry from "@sentry/vue";
import { isProxy, toRaw } from 'vue';
import XLSX from 'xlsx/xlsx';
import RecursiveTreeComponent from '@/components/interfaces/dataRetrieve/treeComponent.vue'

export default {
    name: "dataRetrieve",
    components: {
        RecursiveTreeComponent
    },
    data() {
        return {
            url: "https://backend.vflowtechiot.com",
            db_accessToken: localStorage.getItem("authToken"),
            
            // For setting up tree view and tree selection
            selectionDatas: {},
            queryItems: [],
            
            // For date time
            calendarDateTimeStart: new Date(),      // Stores DateTime
            storeStartDateTime: 0,                  // Stores Time
            calendarDateTimeEnd: new Date(),        // Stores DateTime
            storeEndDateTime: 0,                    // Stores Time
            
            // All drop down list 
            selectPlatform: "Send by Email",        // Drop down list, platform selector
            aggregationSelect: 0,
            
            // For gateway
            GatewayPageNumber: 1,
            GatewayPageSize: 5000,

            // MISC stats
            isLoading: false,
            isSuccess: false,
            isWarningMsg: false,
            isDDLDisabled: false,
            isDownloadDisabled: false,
            EmailAddress: "",
            errorMsg: "",
        }
    },
    mounted() {
        const user = {
            email: localStorage.getItem("loggedUser")
        };
        Sentry.setUser(user);
        this.QueryEmailDownloadList();
    },
    watch: {
        'queryItems': function () {
            // Reset error message everytime tree view is modified.
            this.isSuccess = false;
            this.errorMsg = "";
            // This function is only for widget
            let lst = this.buildDataRetrieved(this.queryItems)
            if (this.selectPlatform === "Download from Widget" && lst[1] !== undefined && lst[0])
            {
                this.isDownloadDisabled = true;
                let widgetInfo = this.buildDataRetrieved(this.queryItems)[1];
                let query = this.url.concat("/api/dash_mgmt/orgs/3/dashboards/");
                query = query.concat(String(widgetInfo));
                fetch(query, {
                    "method": "GET", 
                    "headers": {
                        'Content-Type': 'application/json',
                        'Authorization' : "Bearer ".concat(this.db_accessToken)
                    },
                })
                .then(response => response.json())
                .then(response => {
                    if ("detail" in response)
                    {
                        if ("title" in response)
                        {
                            if (response["title"] === "Unauthorized")
                            {
                                localStorage.removeItem("loggedUser");
                                localStorage.removeItem("authToken");
                                this.$router.push({ name: 'login' });
                            }
                            else 
                            {
                                this.isLoading = false;
                                this.errorMsg = "VFTIoT Server Error," + response["detail"];
                                Sentry.captureMessage("Get Panels API Error Message", {extra:response});
                            }
                        }
                        else 
                        {
                            this.isLoading = false;
                            this.errorMsg = "VFTIoT Server Error," + response["detail"];
                            Sentry.captureMessage("Get Panels API Error Message", {extra:response});
                        }
                    }
                    else 
                    {
                        let temp_panelID = -1;
                        for (let lstIndex in response["panels"])
                        {
                            if (response["panels"][lstIndex]["name"].toUpperCase().includes("DATA RETRIEVAL"))
                            {
                                temp_panelID = response["panels"][lstIndex]["id"];
                                widgetInfo = [widgetInfo, temp_panelID];
                            }
                        }
                        if(temp_panelID !== -1)
                        {
                            let query = this.url.concat("/api/dash_mgmt/orgs/3/dashboards/");
                            query = query.concat(String(widgetInfo[0]));
                            query = query.concat("/panels/");
                            query = query.concat(String(widgetInfo[1]));
                            fetch(query, {
                                "method": "GET", 
                                "headers": {
                                    'Content-Type': 'application/json',
                                    'Authorization' : "Bearer ".concat(this.db_accessToken)
                                },
                            })
                            .then(response => response.json())
                            .then(response => {
                                if ("detail" in response)
                                {
                                    if ("title" in response)
                                    {
                                        if (response["title"] === "Unauthorized")
                                        {
                                            localStorage.removeItem("loggedUser");
                                            localStorage.removeItem("authToken");
                                            this.$router.push({ name: 'login' });
                                        }
                                        else 
                                        {
                                            this.isLoading = false;
                                            this.errorMsg = "VFTIoT Server Error," + response["detail"];
                                            Sentry.captureMessage("Get Widget Instance API Error Message", {extra:response});
                                        }
                                    }
                                    else 
                                    {
                                        this.isLoading = false;
                                        this.errorMsg = "VFTIoT Server Error," + response["detail"];
                                        Sentry.captureMessage("Get Widget Instance API Error Message", {extra:response});
                                    }
                                }
                                else
                                {
                                    // Widget Info = [dashboard_id, panels_id, widget_id, widget_instance]
                                    widgetInfo = widgetInfo.concat([response["widgets"][0]["widget_id"], response["widgets"][0]["id"], this.queryItems[1]])
                                    this.queryItems = [false, widgetInfo];
                                    this.isDownloadDisabled = false;
                                }
                            })
                            .catch(err => {
                                this.isLoading = false;
                                this.errorMsg = "Connection Error? " + err;
                                Sentry.captureMessage("(Error Occured During Fetch) Get Widget Instance Error: " + err);
                            });
                        }
                    }
                })
                .catch(err => {
                    this.isLoading = false;
                    this.errorMsg = "Connection Error? " + err;
                    Sentry.captureMessage("(Error Occured During Fetch) Get Panels API Error: " + err);
                });
            }
        }
    },
    methods: {
        //==================================// MISC Functions //==================================//
        extractData(val) 
        {                          // Used to extract data to raw and retrieve entities
            if (isProxy(val))
                return toRaw(val)
            else
                return val
        },
        getTimeStamp(dateTimeVal, startDate)        // Retrieve start and end date timestamp
        {
            if (dateTimeVal !== null)
            {
                if (Object.prototype.toString.call(dateTimeVal) != '[object Date]')
                    dateTimeVal = new Date(dateTimeVal+"+08:00");
                if (startDate)
                    this.storeStartDateTime = dateTimeVal.getTime();
                else
                    this.storeEndDateTime = dateTimeVal.getTime();
            }
            if (startDate)
                return this.storeStartDateTime;
            else
                return this.storeEndDateTime;
        },
        getTimeStampList()                          // Return as a list for timestamp
        {
            return [this.getTimeStamp(this.calendarDateTimeStart, true), this.getTimeStamp(this.calendarDateTimeEnd, false)];
        },
        ChangeSelection(){                           // If switch of platform is made, clear selected sensors/unit
            this.errorMsg = "";
            this.queryItems = [];
            this.selectionDatas = [];
            if (this.selectPlatform === "Send by Email")
                this.QueryEmailDownloadList();
            else if (this.selectPlatform === "Download from Gateway")
                this.QueryGatewayList();
            else if (this.selectPlatform === "Download from Widget")
                this.QueryDashboardList();
        },
        fillDoubleArrayWithZero(lst)
        {
            let maxRow = 0;
            for (let index1 = 0; index1 < lst.length; index1++)
            {
                if (lst[index1].length > maxRow)
                    maxRow = lst[index1].length;
            }
            for (let index1 = 0; index1 < lst.length; index1++)
            {
                for (let index2 = 0; index2 < maxRow; index2++)
                {
                    if(lst[index1][index2] == null || lst[index1] == undefined)
                        lst[index1][index2] = 0;
                }
            }
            return lst
        },
        transposeDoubleArray(lst){
            let ls_temp = [];
            let col_length = lst[0].length;
            let row_length = lst.length;
            for (let temp_col = 0; temp_col < col_length; temp_col ++)
            {
                ls_temp.push(new Array(row_length).fill(0));
            }
            for (let index1 = 0; index1 < lst.length; index1++)
            {   
                for (let index2 = 0; index2 < lst[index1].length; index2++)
                {
                    ls_temp[index2][index1] = lst[index1][index2];
                }
            }
            lst = ls_temp;
            return ls_temp
        },
         //==================================// QUERY Functions //==================================//
        QueryEmailDownloadList()                            // Query for list of units and senors
        {
            this.isDDLDisabled = true;
            let query = this.url.concat("/api/reports/orgs/3/entities")
            fetch(query, {
                "method": "GET", 
                "headers": {
                    'Content-Type': 'application/json',
                    'Authorization' : "Bearer ".concat(this.db_accessToken)
                }
            })
            .then(response => response.json())
            .then(response => {
                if ("detail" in response)
                {
                    if ("title" in response)
                    {
                        if (response["title"] === "Unauthorized")
                        {
                            localStorage.removeItem("loggedUser");
                            localStorage.removeItem("authToken");
                            this.$router.push({ name: 'login' });
                        }
                        else 
                        {
                            this.isLoading = false;
                            this.errorMsg = "VFTIoT Server Error," + response["detail"];
                            Sentry.captureMessage("Retrieve Dashboard List API Error Message", {extra:response});
                        }
                    }
                    else 
                    {
                        this.isLoading = false;
                        this.errorMsg = "VFTIoT Server Error," + response["detail"];
                        Sentry.captureMessage("Retrieve Dashboard List API Error Message", {extra:response});
                    }
                }
                else 
                {
                    Sentry.captureMessage("Query Made - Get Sensors List for Email", {extra:{query: query}});
                    this.isDDLDisabled = false;
                    this.selectionDatas = response["entities"];
                }
            })
            .catch(err => {
                this.isLoading = false;
                this.errorMsg = "Connection Error? " + err;
                Sentry.captureMessage("(Error Occured During Fetch) Retrieve Dashboard List Error: " + err)
            });    
        },
        QueryDashboardList()
        {
            this.isDDLDisabled = true;
            let query = this.url.concat("/api/dash_mgmt/orgs/3/dashboards?")
            query = query.concat("page_size=1000&from=0")
            fetch(query, {
                "method": "GET", 
                "headers": {
                    'Content-Type': 'application/json',
                    'Authorization' : "Bearer ".concat(this.db_accessToken)
                },
            })
            .then(response => response.json())
            .then(response => {
                if ("detail" in response)
                {
                    if ("title" in response)
                    {
                        if (response["title"] === "Unauthorized")
                        {
                            localStorage.removeItem("loggedUser");
                            localStorage.removeItem("authToken");
                            this.$router.push({ name: 'login' });
                        }
                        else 
                        {
                            this.isLoading = false;
                            this.errorMsg = "VFTIoT Server Error," + response["detail"];
                            Sentry.captureMessage("Retrieve Widget List API Error Message", {extra:response});
                        }
                    }
                    else 
                    {
                        this.isLoading = false;
                        this.errorMsg = "VFTIoT Server Error," + response["detail"];
                        Sentry.captureMessage("Retrieve Widget List API Error Message", {extra:response});
                    }
                }
                else 
                {
                    Sentry.captureMessage("Query Made - Get Dashboard List", {extra:{query: query}});
                    let temp_entities = [];
                    for (let lstIndex in response["dashboards"])
                    {
                        let name = response["dashboards"][lstIndex]["name"].toUpperCase();
                        if (name.includes("SERVICE") || name.includes("ANALYSIS") || name.includes("DEBUG"))
                        {
                            temp_entities.push(response["dashboards"][lstIndex]);
                        }
                    }
                    this.isDDLDisabled = false;
                    this.selectionDatas = temp_entities;
                }
            })
            .catch(err => {
                this.isLoading = false;
                this.errorMsg = "Connection Error? " + err;
                Sentry.captureMessage("(Error Occured During Fetch) Retrieve Widget List Error: " + err);
            });
        },
        QueryGatewayList()                          // Query for list of gateways
        {
            this.isDDLDisabled = true;
            let query = this.url.concat("/api/iot_mgmt/orgs/3/projects/70/gateways?")
            query = query.concat("page_size=1000&from=0")
            Sentry.captureMessage("Query Made - Get Gateway List", {extra:{query: query}});
            fetch(query, {
                "method": "GET", 
                "headers": {
                    'Content-Type': 'application/json',
                    'Authorization' : "Bearer ".concat(this.db_accessToken)
                },
            })
            .then(response => response.json())
            .then(response => {
                if ("detail" in response)
                {
                    if ("title" in response)
                    {
                        if (response["title"] === "Unauthorized")
                        {
                            localStorage.removeItem("loggedUser");
                            localStorage.removeItem("authToken");
                            this.$router.push({ name: 'login' });
                        }
                        else 
                        {
                            this.isLoading = false;
                            this.errorMsg = "VFTIoT Server Error," + response["detail"];
                            Sentry.captureMessage("Retrieve Gateway List API Error Message", {extra:response});
                        }
                    }
                    else 
                    {
                        this.isLoading = false;
                        this.errorMsg = "VFTIoT Server Error," + response["detail"];
                        Sentry.captureMessage("Retrieve Gateway List API Error Message", {extra:response});
                    }
                }
                else
                {
                    this.isDDLDisabled = false;
                    this.selectionDatas = response["gateways"];
                }
            })
            .catch(err => {
                this.APIError = err;
                Sentry.captureMessage("(Fetch Function) Retrieve Gateway List Error" + err)
            });
        },
         //================================// DOWNLOAD Functions //================================//
        sendAndCollectData() 
        {
            this.isDownloadDisabled = true;
            let currTime = new Date();
            let timelst = this.getTimeStampList();

            if(timelst[0] > timelst[1] || timelst[1] > currTime.getTime())
            {
                this.isSuccess = false;
                this.errorMsg = "Invalid date range.";
            }
            else
            {
                if(this.selectPlatform === "Send by Email")
                {
                    let lst = this.buildDataRetrieved(this.queryItems)[1];
                    if (lst.length > 0)
                    {
                        if (this.EmailAddress !== "")
                        {
                            this.errorMsg = "";
                            this.isLoading = true;
                            this.isSuccess = false;
                            let query = this.url.concat("/api/reports/orgs/3/download_excel_report")
                            fetch(query, {
                                "method": "POST", 
                                "headers": {
                                    'Content-Type': 'application/json',
                                    'Authorization' : "Bearer ".concat(this.db_accessToken)
                                },
                                "body": JSON.stringify({
                                    "entities": lst,
                                    "date_to": String(timelst[1]),
                                    "date_from": String(timelst[0]),
                                    "email_to": [this.EmailAddress]
                                })
                            })
                            .then(response => response.json())
                            .then(response => {
                                if ("detail" in response)
                                {
                                    if ("title" in response)
                                    {
                                        if (response["title"] === "Unauthorized")
                                        {
                                            localStorage.removeItem("loggedUser");
                                            localStorage.removeItem("authToken");
                                            this.$router.push({ name: 'login' });
                                        }
                                        else 
                                        {
                                            this.isLoading = false;
                                            this.isDownloadDisabled = false;
                                            this.errorMsg = "VFTIoT Server Error," + response["detail"];
                                            Sentry.captureMessage("Download Excel API Error Message", {extra:response});
                                        }
                                    }
                                    else 
                                    {
                                        this.isLoading = false;
                                        this.isDownloadDisabled = false;
                                        this.errorMsg = "VFTIoT Server Error," + response["detail"];
                                        Sentry.captureMessage("Download Excel API Error Message", {extra:response});
                                    }
                                }
                                else
                                {
                                    Sentry.captureMessage("Query Made - Download Data With Email", {extra:{query: query, entities: lst}});
                                    this.isLoading = false;
                                    this.isSuccess = true;
                                    this.errorMsg = "";
                                    this.isDownloadDisabled = false;
                                }
                            })
                            .catch(err => {
                                this.isLoading = false;
                                this.isDownloadDisabled = false;
                                this.errorMsg = "Connection Error? " + err;
                                Sentry.captureMessage("(Error Occured During Fetch) Download Excel Error: " + err)
                            });
                        }
                        else
                        {
                            this.isSuccess = false;
                            this.isDownloadDisabled = false;
                            this.errorMsg = "Please input an email."    
                        }
                    }
                    else
                    {
                        this.isSuccess = false;
                        this.isDownloadDisabled = false;
                        this.errorMsg = "Please select a sensor."
                    }
                }
                else if(this.selectPlatform === "Download from Widget")
                {
                    if (this.queryItems.length > 0)
                    {
                        this.errorMsg = "";
                        this.isLoading = true;
                        this.isSuccess = false;
                        let aggreFunct = "no"
                        if (this.aggregationSelect != 0)
                            aggreFunct = "average"
                        // Widget Info = [dashboard_id, panels_id, widget_id, widget_instance]
                        let query = this.url.concat("/api/dash_mgmt/orgs/3/dashboards/");
                        query = query.concat(String(this.queryItems[1][0]));
                        query = query.concat("/panels/");
                        query = query.concat(String(this.queryItems[1][1]));
                        fetch(query, {
                            "method": "PUT", 
                            "headers": {
                                'Content-Type': 'application/json',
                                'Authorization' : "Bearer ".concat(this.db_accessToken)
                            },
                            "body": JSON.stringify({
                                "filter_metadata":{
                                    "aggregate_func":aggreFunct,
                                    "from_date":timelst[0],
                                    "to_date":timelst[1],
                                    "group_interval":this.aggregationSelect,
                                    "group_interval_type":"minute",
                                    "type":"historical",
                                    "last":"custom"
                                }
                            })
                        })
                        .then(response => response.json())
                        .then(response => {
                            if ("detail" in response)
                            {
                                if ("title" in response)
                                {
                                    if (response["title"] === "Unauthorized")
                                    {
                                        localStorage.removeItem("loggedUser");
                                        localStorage.removeItem("authToken");
                                        this.$router.push({ name: 'login' });
                                    }
                                    else 
                                    {
                                        this.isLoading = false;
                                        this.isDownloadDisabled = false;
                                        this.errorMsg = "VFTIoT Server Error," + response["detail"];
                                        Sentry.captureMessage("Setting Up Widget API Error Message", {extra:response});
                                    }
                                }
                                else 
                                {
                                    this.isLoading = false;
                                    this.isDownloadDisabled = false;
                                    this.errorMsg = "VFTIoT Server Error," + response["detail"];
                                    Sentry.captureMessage("Setting Up Widget API Error Message", {extra:response});
                                }
                            }
                            else
                            {
                                // Widget Info = [dashboard_id, panels_id, widget_id, widget_instance]
                                let query = this.url.concat("/api/dash_mgmt/orgs/3/panels/");
                                query = query.concat(String(this.queryItems[1][1]));
                                query = query.concat("/widgets/");
                                query = query.concat(String(this.queryItems[1][2]));
                                query = query.concat("/widget_instances/");
                                query = query.concat(String(this.queryItems[1][3]));
                                fetch(query, {
                                    "method": "GET", 
                                    "headers": {
                                        'Content-Type': 'application/json',
                                        'Authorization' : "Bearer ".concat(this.db_accessToken)
                                    },
                                })
                                .then(response => response.json())
                                .then(response => {
                                    if ("detail" in response)
                                    {
                                        if ("title" in response)
                                        {
                                            if (response["title"] === "Unauthorized")
                                            {
                                                localStorage.removeItem("loggedUser");
                                                localStorage.removeItem("authToken");
                                                this.$router.push({ name: 'login' });
                                            }
                                            else 
                                            {
                                                this.isLoading = false;
                                                this.isDownloadDisabled = false;
                                                this.errorMsg = "VFTIoT Server Error," + response["detail"];
                                                Sentry.captureMessage("Download Widget API Error Message", {extra:response});
                                            }
                                        }
                                        else 
                                        {
                                            this.isLoading = false;
                                            this.isDownloadDisabled = false;
                                            this.errorMsg = "VFTIoT Server Error," + response["detail"];
                                            Sentry.captureMessage("Download Widget API Error Message", {extra:response});
                                        }
                                    }
                                    else
                                    {
                                        Sentry.captureMessage("Query Made - Download From Widget Data", {extra:{query: query}});
                                        let readDateTime = true;
                                        let dateTimeRead = false;
                                        let ls = []
                                        let ls_date = []
                                        let timestamp = []
                                        let column = ["date (GMT 8+)", "time_stamp"]
                                        for (let lstIndex in response["series"])
                                        {
                                            let ls_temp = []
                                            column.push(response["series"][lstIndex]["name"])
                                            for (let lstIndex2 in response["series"][lstIndex]["data"])
                                            {
                                                if (readDateTime && response["series"][lstIndex]["data"][lstIndex2].length != 0)
                                                {
                                                    let tempDate = response["series"][lstIndex]["data"][lstIndex2][0];
                                                    ls_date.push((new Date(tempDate)).toLocaleString());
                                                    timestamp.push(tempDate);
                                                    dateTimeRead = true;
                                                }
                                                ls_temp.push(response["series"][lstIndex]["data"][lstIndex2][1])
                                            }
                                            if (readDateTime && dateTimeRead)
                                            {
                                                ls.push(ls_date);
                                                ls.push(timestamp);
                                                readDateTime = false;
                                            }
                                            ls.push(ls_temp)
                                        }
                                        this.fillDoubleArrayWithZero(ls);
                                        let ls2 = this.transposeDoubleArray(ls)
                                        ls2.push(column)
                                        let workbook = XLSX.utils.book_new(),
                                        worksheet = XLSX.utils.aoa_to_sheet(ls2.reverse());
                                        workbook.SheetNames.push("Generated Data");
                                        workbook.Sheets["Generated Data"] = worksheet;
                                        XLSX.writeFile(workbook, this.queryItems[1][4]+".xlsx");
                                        this.isLoading = false;
                                        this.isSuccess = true;
                                        this.errorMsg = "";
                                        this.isDownloadDisabled = false;
                                    }
                                })
                                .catch(err => {
                                    this.isLoading = false;
                                    this.isDownloadDisabled = false;
                                    this.errorMsg = "Connection Error? " + err;
                                    Sentry.captureMessage("(Error Occured During Fetch) Download Widget Error: " + err);
                                });
                            }
                        })
                        .catch(err => {
                            this.isLoading = false;
                            this.isDownloadDisabled = false;
                            this.errorMsg = "Connection Error? " + err;
                            Sentry.captureMessage("(Error Occured During Fetch) Setting Up Widget Error: " + err);
                        });
                    }
                    else
                    {
                        this.isSuccess = false;
                        this.isDownloadDisabled = false;
                        this.errorMsg = "Please select a widget."
                    }
                }
                else if (this.selectPlatform == "Download from Gateway")
                {
                    if(this.queryItems.length > 0)
                    {
                        let data = [];
                        let column = [];
                        this.errorMsg = "";
                        this.isLoading = true;
                        this.isSuccess = false;
                        this.gatewayRecursiveCall(data, column, 1, timelst);
                    }
                    else
                    {
                        this.isSuccess = false;
                        this.isDownloadDisabled = false;
                        this.errorMsg = "Please select a gateway."
                    }
                }
            }
        },
        gatewayRecursiveCall(data, column, page_number, timelst)
        {
            let query = this.url.concat("/api/iot_mgmt/orgs/3/projects/70/gateways/");
            query = query.concat(String(this.queryItems[2]));
            query = query.concat("/data_dump_index?");
            query = query.concat("page_size=");
            query = query.concat(this.GatewayPageSize);
            query = query.concat("&page_number=");
            query = query.concat(page_number);
            query = query.concat("&to_date=");
            query = query.concat(timelst[1]);
            query = query.concat("&from_date=");
            query = query.concat(timelst[0]);
            this.GatewayPageNumber = page_number;
            Sentry.captureMessage("Query Made - Download Data from Gateway, Page " + page_number, {extra:{query: query}});
            fetch(query, {
                "method": "GET", 
                "headers": {
                    'Content-Type': 'application/json',
                    'Connection': 'keep-alive',
                    'Authorization' : "Bearer ".concat(this.db_accessToken)
                }
            })
            .then(response => response.json())
            .then(response => {
                if ("detail" in response)
                {
                    if ("title" in response)
                    {
                        if (response["title"] === "Unauthorized")
                        {
                            localStorage.removeItem("loggedUser");
                            localStorage.removeItem("authToken");
                            this.$router.push({ name: 'login' });
                        }
                        else 
                        {
                            this.isLoading = false;
                            this.isDownloadDisabled = false;
                            this.errorMsg = "VFTIoT Server Error," + response["detail"];
                            Sentry.captureMessage("Download Gateway API Error Message", {extra:response});
                        }
                    }
                    else 
                    {
                        this.isLoading = false;
                        this.isDownloadDisabled = false;
                        this.errorMsg = "VFTIoT Server Error," + response["detail"];
                        Sentry.captureMessage("Download Gateway API Error Message", {extra:response});
                    }
                }
                else
                {
                    for (let dd_value in response["data_dumps"])
                    {
                        let temp_dd_value = [];
                        if(dd_value == 0 && data.length == 0)
                        {
                            column.push("date (GMT 8+)");
                            column.push("time_stamp");
                            for (let dd2_value in response["data_dumps"][dd_value]["data"]["assets_params"])
                                column.push(dd2_value);
                        }
                        let tempDate = response["data_dumps"][dd_value]["data"]["timestamp"] * 1000;
                        temp_dd_value.push((new Date(tempDate)).toLocaleString());
                        temp_dd_value.push(response["data_dumps"][dd_value]["data"]["timestamp"]);
                        for (let dd2_value in response["data_dumps"][dd_value]["data"]["assets_params"])
                                temp_dd_value.push(response["data_dumps"][dd_value]["data"]["assets_params"][dd2_value]);
                        data.push(temp_dd_value);
                    }
                    if (response["data_dumps"].length == this.GatewayPageSize)
                    {
                        this.gatewayRecursiveCall(data, column, page_number+1, timelst);
                    }
                    else
                    {
                        data.push(column);
                        let workbook = XLSX.utils.book_new(),
                        worksheet = XLSX.utils.aoa_to_sheet(data.reverse());
                        workbook.SheetNames.push("Generated Data");
                        workbook.Sheets["Generated Data"] = worksheet;
                        XLSX.writeFile(workbook, this.queryItems[1]+".xlsx");
                        this.isLoading = false;
                        this.isSuccess = true;
                        this.errorMsg = "";
                        this.isDownloadDisabled = false;
                    }
                }
            })
            .catch(err => {
                this.isLoading = false;
                this.isDownloadDisabled = false;
                this.errorMsg = "Connection Error? " + err;
                Sentry.captureMessage("(Error Occured During Fetch) Download Gateway Error: " + err);
            });
        },
        // To collect recursive data from email sensor download.
        buildDataRetrieved(tempData) 
        {
            if (this.selectPlatform === "Send by Email")
            {
                if (tempData.length === 0)
                {
                    return [true, []]
                }
                else if (tempData[0] === "rb" && tempData.length > 3)
                {
                    let tempVal = this.buildDataRetrieved(tempData[3]);
                    if (tempVal[0] === true)
                        return tempVal;
                    else
                    {
                        let tempRtn = [];
                        if (this.selectPlatform === "Send by Email")
                        {
                            for (let lstVal of tempVal)
                            {
                                tempRtn.push(JSON.parse(JSON.stringify({
                                    "sensor_id": tempData[2],
                                    "param_uuid": lstVal
                                })));
                            }
                        }
                        else
                        {
                            tempRtn.push(tempVal)
                        } 
                        return [true, tempRtn];
                    }
                } 
                else
                {
                    let tempLst = [];
                    for (let sensorInfo of tempData)
                    {
                        if(sensorInfo[0] === "cb")
                            tempLst.push(sensorInfo[2]);
                    }
                    return tempLst;
                }
            }
            else 
            {
                return [true, tempData[2]];
            }
        },
        handleMessageFromChild(message) 
        {
            this.queryItems = this.extractData(message);
        },
    }
}
</script>

<template>
    <div class="DR-MainContainer">
        <div class="DR-SubContainer">
            <div class="DR-TreeContainer">
                <div class="titleText1">Select Sensors or Device</div>
                <RecursiveTreeComponent :isDownloadDisabled="isDownloadDisabled" :selectionType="selectPlatform" :items="selectionDatas" @update-queryItem="handleMessageFromChild" />
            </div>
        </div>
        <div class="DR-SubContainer2">
            <div class="DR-SubContainer3">
                <div class="DR-SubContainer4">
                    <label class="titleText1">Select Date:</label>
                    <div class="DatePickerContainer3" v-show="!isDownloadDisabled">
                        <label class="DPlbl2">Start Date:</label>
                        <VueCtkDateTimePicker class="calendarPicker" v-model="calendarDateTimeStart" />
                    </div>
                    <div class="DatePickerContainer3" v-show="!isDownloadDisabled">
                        <label class="DPlbl2">End Date:</label>
                        <VueCtkDateTimePicker class="calendarPicker" v-model="calendarDateTimeEnd" />
                    </div>
                    <div class="DatePickerContainer3" v-show="isDownloadDisabled">
                        <label class="DPlbl2">Locking Date Selection...</label>
                    </div>
                </div>
            </div>
            <!-- <div class="DR-SubContainer3">
                <div class="DR-SubContainer4">
                    <div class="titleText1">Summary:</div>
                    <div class="selectedUnit">Selected Unit:</div>
                    <div class="selectUnitVal" v-if="checkedUnit.length>0">{{ checkedUnit[0] }}</div>
                    <div class="selectUnitVal1" v-if="checkedUnitSub.length>0">&#x1F872; {{ checkedUnitSub[0] }}</div>
                    <div class="selectedUnit">Selected Sensor:</div>
                    <div class="selectUnitVal" v-for="val in cb2checkedvalue">{{ val[0] }}</div>
                    <div class="selectedUnit">Selected Sending Platform:</div>
                    <div class="selectUnitVal" v-if="selectPlatform != null">{{ selectPlatform }}</div>
                </div>
            </div> -->
            <div class="DR-SubContainer3">
                <div class="DR-SubContainer4">
                    <select class="pltSelect" v-model="selectPlatform" :disabled="isDDLDisabled || isDownloadDisabled" @change="ChangeSelection()">
                        <option value="Send by Email">Send by Email</option>
                        <option value="Download from Widget">Download from Widget</option>
                        <option value="Download from Gateway">Download from Gateway</option>
                    </select>
                    <div v-if="this.selectPlatform == 'Send by Email'">
                        <div class="DPlbl2">Email Address:</div>
                        <input class="inputClass" type="text" v-model="EmailAddress"/>
                    </div>
                    <div v-else-if="this.selectPlatform == 'Download from Widget'">
                        <div v-if="isWarningMsg" class="WidgetInstructions" @click="isWarningMsg= !isWarningMsg">The device selection will display a list of service dashboard. If the selected dashboard do not allow you to download data, then the dashboard do not have an existing widget for you to download data from.<br><br> How this function works is that the interface will search for a panel tab named "DATA RETRIEVAL". The program will only read all parameters from the first widget added into "DATA RETRIEVAL" panel tab.</div>
                        <div v-else class="WidgetInstructions" @click="isWarningMsg= !isWarningMsg">Instructions...</div>
                        <div class="DPlbl2">Aggregation:</div>
                        <select class="agreSelect" v-model="aggregationSelect" :disabled="isDDLDisabled || isDownloadDisabled">
                            <option value="0">No</option>
                            <option value="5">5 minutes</option>
                            <option value="30">30 minutes</option>
                        </select>
                    </div>
                    <div v-if="errorMsg!==''" class="ErrorMsg">{{ errorMsg }}</div>
                    <div v-if="ifDateWarning" class="WarningsMsg">(!) Your selected date range is more than a week, this may cause delays or failure in data retrieval.</div>
                    <div v-if="isSuccess" class="successMsg">Parameters query success.</div>
                    <div v-if="isLoading && this.selectPlatform !== 'Download from Gateway'" class="Msg">Communicating with server.</div>
                    <div v-if="isLoading && this.selectPlatform === 'Download from Gateway'" class="Msg">Communicating with server, page {{ GatewayPageNumber }}...</div>
                    <button class="submitbtn" :disabled="isDownloadDisabled" @click="sendAndCollectData()">Build Data/Receive Email</button>
                </div>
            </div>
        </div>
    </div>
</template>

<style scoped>
.DR-MainContainer {
    display: flex;
    min-height: 91.8vh;
    flex-direction: row;
}
.DR-SubContainer {
    width: 50%;
    padding: 1rem;
}
.DR-SubContainer2 {
    width: 50%;
}
.DR-SubContainer3 {
    gap: 1rem;
    display: flex;
    padding: 1rem;
    flex-direction: column;
}
.DR-SubContainer4 {
    /* border: solid; */
    display: flex;
    border-radius: 1rem;
    flex-direction: column;
    padding: 0.5rem 1.5rem;
    background-color: #FAF9F9;
    box-shadow: rgba(0, 0, 0, 0.25) 0px 54px 55px, rgba(0, 0, 0, 0.12) 0px -12px 30px, rgba(0, 0, 0, 0.12) 0px 4px 6px, rgba(0, 0, 0, 0.17) 0px 12px 13px, rgba(0, 0, 0, 0.09) 0px -3px 5px;
}
.titleText1 {
    padding: 0rem 0.5rem;
    margin: 0.3rem 0rem;
    font-size: 2rem;
    border-bottom: solid;
    border-width: 0.1rem;
    border-radius: 0.3rem;
}
.DR-TreeContainer {
    border-width: 0.1rem;
    border-radius: 1.2rem;
    padding: 0.5rem 1.5rem;
    background-color: #FAF9F9;
    box-shadow: rgba(0, 0, 0, 0.25) 0px 54px 55px, rgba(0, 0, 0, 0.12) 0px -12px 30px, rgba(0, 0, 0, 0.12) 0px 4px 6px, rgba(0, 0, 0, 0.17) 0px 12px 13px, rgba(0, 0, 0, 0.09) 0px -3px 5px;
}
.calendarPicker {
    border: solid;
    min-width: 4rem;
}
.submitbtn {
    width: 100%;
    height: 3rem;
    font-size: 1.3rem;
    border-radius: 1rem;
}
.submitbtn:active {
    transform: scale(0.95);
}
.pltSelect {
    width: 100%;
    height: 2rem;
    font-size: 1.3rem;
    border-radius: 0.5rem;
    margin-bottom: 1rem;
    padding-left: 2rem;
}
.agreSelect{
    width: 100%;
    height: 2rem;
    font-size: 1.1rem;
    border-radius: 0.5rem;
    margin-bottom: 1rem;
    padding-left: 1rem;
}
.inputClass {
    width: 100%;
    margin-bottom: 1rem;
    margin-top: 0.2rem;
    border-width: 0.1rem;
    border-radius: 0.5rem;
    padding: 0.5rem 1rem;
    font-size: 1.2rem;
    height: 2rem;
}
.WidgetInstructions {
    display: flex;
    text-align: justify;
    margin-bottom: 1rem;
    color: rgb(189, 140, 34);
    cursor: pointer;
}
.WarningsMsg {
    display: flex;
    text-align: justify;
    margin-bottom: 1rem;
    color: rgb(236, 127, 37);
}
.ErrorMsg {
    color: rgb(230, 85, 85);
    font-size: 1.2rem;
    text-align: center;
    margin-bottom: 1rem;
    text-decoration: underline;
    text-underline-offset: 0.5rem;
}
.Msg
{
    font-size: 1.2rem;
    text-align: center;
    margin-bottom: 1rem;
    text-decoration: underline;
    text-underline-offset: 0.5rem;
}
.successMsg
{
    color: rgb(114, 216, 136);
    font-size: 1.2rem;
    text-align: center;
    margin-bottom: 1rem;
    text-decoration: underline;
    text-underline-offset: 0.5rem;
}
@media (max-width: 800px) {
    .DR-SubContainer {
        width: 100%;
    }
    .DR-MainContainer {
        flex-direction: column;
    }
    .DR-SubContainer2 {
        width: 100%;
    }
}
</style>