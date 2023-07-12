<script>
import * as Sentry from "@sentry/vue";
import { ref, computed, onMounted } from 'vue';
import { isProxy, toRaw } from 'vue';
import XLSX, { read } from 'xlsx/xlsx';

export default {
    name: "dataRetrieve",
    // props: {
    //     accessToken: String, 
    //     entitiesSelector: Object
    // },
    data() {
        return {
            url: "https://backend.vflowtechiot.com",
            db_accessToken: localStorage.getItem("authToken"),
            db_entitiesSelector: [],                // Store entire list of entities retrieved from API. No references is made to this
            checkedUnit: [],                        // Stores the first layer of entities selected (Total 3)
            cb2checkedvalue: [],                    // Stores the second layer of entities selected (Total 3)
            cb2ischecked: [],                       //      A reference for checkbox to be checked or unchecked
            checkedUnitSub: [],                     //      This is used to decide if the third layer of checkbox should be displayed
            cb3checkedvalue: [],                    // Stores the third layer of entities selected (Total 3). This is used to query for sensor
            cb3ischecked: [],                       //      A reference for checkbox to be checked or unchecked
            calendarDateTimeStart: new Date(),      // Stores DateTime
            storeStartDateTime: 0,                  // Stores Time
            calendarDateTimeEnd: new Date(),        // Stores DateTime
            storeEndDateTime: 0,                    // Stores Time
            selectPlatform: "Send by Email",        // Drop down list, platform selector
            ifLoading: false,
            isNoEmail: false,                       // Error reference if there is no email entered
            ifSuccess: false,                       // Success reference
            ifWarningMsg: false,
            ifDateWarning: false,
            isInvalidDate: false,                   // Error reference if date range is not valid
            isNoSensorSelected: false,              // Error reference if there is no sensor selected
            isDownloadDisabled: false,
            APIError: "",                           // Stores API Error message, message will only be printed on console
            EmailAddress: "",                       // Stores email to be sent
            aggregationSelect: 0,
            isDDLDisabled: true,                    // Disable drop down list reference
            GatewayPageNumber: 1,                   // A Global storage for Gateway
            GatewayPageSize: 5000,                  // A Global storage for Gateway
            dashboardID: -1,
            panelID: -1,
            widgetID: -1,
            widgetInstance: -1,
            adBlockerActive: false,
            sseEvent: {},
            sseMessage: [],
        }
    },
    mounted() {
        this.checkAdBlocker();
        const user = {
            email: localStorage.getItem("loggedUser")
        };
        Sentry.setUser(user);
        this.sendEmailSelection();
    },
    watch: {
        'checkedUnit': function (){
            if (this.selectPlatform == "Download from Widget" && this.checkedUnit[1] != undefined)
            {
                console.log(this.checkedUnit);
                this.isDownloadDisabled = true;
                this.dashboardID = this.checkedUnit[1];
                let query = this.url.concat("/api/dash_mgmt/orgs/3/dashboards/");
                query = query.concat(String(this.checkedUnit[1]));
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
                        this.APIError = "Server Error," + response["detail"];
                        Sentry.captureMessage("Get Panels API Error Message", {extra:response})
                    }
                    else
                    {
                        let temp_panelID = -1;
                        for (let lstIndex in response["panels"])
                        {
                            if (response["panels"][lstIndex]["name"].toUpperCase().includes("DATA RETRIEVAL"))
                            {
                                temp_panelID = response["panels"][lstIndex]["id"];
                                this.panelID = temp_panelID;
                            }
                        }
                        if(temp_panelID != -1)
                        {
                            let query = this.url.concat("/api/dash_mgmt/orgs/3/dashboards/");
                            query = query.concat(String(this.checkedUnit[1]));
                            query = query.concat("/panels/");
                            query = query.concat(String(temp_panelID));
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
                                    this.APIError = "Server Error," + response["detail"]
                                    Sentry.captureMessage("Get Widget Instance API Error Message", {extra:response})
                                }
                                else
                                {
                                    this.widgetID = response["widgets"][0]["widget_id"];
                                    this.widgetInstance = response["widgets"][0]["id"];
                                    this.isDownloadDisabled = false;
                                }
                            })
                            .catch(err => {
                                this.APIError = err;
                                Sentry.captureMessage("(Fetch Function) Get Widget Instance Error: " + err)
                            });
                        }
                    }
                })
                .catch(err => {
                    this.APIError = err;
                    Sentry.captureMessage("(Fetch Function) Get Panels API Error: " +  err)
                });
            }
        }
    },
    methods: {
        checkAdBlocker() {
//            let data = new FormData();
//            data.append('test', 'sentry-blocked');
//            navigator.sendBeacon('https://o4505362450546688.ingest.sentry.io/api/4505362452971520/envelope/?sentry_key=2e5752ad299549429e0d3928658b5ab4&sentry_version=7&sentry_client=sentry.javascript.vue%2F7.55.2', data);
//
//            setTimeout(() => {
//                if (!this.adBlockerActive) {
//                    this.adBlockerActive = true;
//                }
//            }, 100);
        },
        extractData(val){                           // Used to extract data to raw and retrieve entities
            if (isProxy(val))
                return toRaw(val)["entities"];
        },
        getNameID(name, id, parentid){return [name, id, parentid];},         // Store name, id, and parent_id of selected sensor
        lstToNameIDString(lst) {return lst[0]+lst[1];},
        printValues(val) {console.log(val);},       // A debug function
        clearSubEntity(){                           // If switch of platform is made, clear selected sensors/unit
            this.checkedUnit = [];
            this.cb2checkedvalue = [];
            this.db_entitiesSelector = [];
            if (this.selectPlatform == "Send by Email")
                this.sendEmailSelection()
            else if (this.selectPlatform == "Download from Gateway")
                this.sendGatewaySelection()
            else if (this.selectPlatform == "Download from Widget")
                this.sendWidgetSelectionLayer1() 
            this.resetMessage();
        },
        cb1uncheck(val){                            // If first layer of checkbox is unchecked
            if (this.lstToNameIDString(val) == this.lstToNameIDString(this.checkedUnit))
                this.checkedUnit = [];
            this.cb2checkedvalue = [];
            this.cb3checkedvalue = [];
            this.checkedUnitSub = false;
            this.resetMessage();
        },
        cb2uncheck(val){                            // If second layer of checkbox is unchecked
            if (this.lstToNameIDString(val) == this.lstToNameIDString(this.checkedUnitSub))
                this.checkedUnitSub = false;
            this.cb3checkedvalue = [];
            this.resetMessage();
        },
        cb2storeValue(lst){                         // Store checkbox value if does not exist and remove if exist
            let index = -1;
            for (let value in this.cb2checkedvalue)
            {
                if (lst[0]==this.cb2checkedvalue[value][0] && lst[1]==this.cb2checkedvalue[value][1])
                {
                    index = value;
                    break;
                }
            }
            if (index == -1)
                this.cb2checkedvalue.push(lst);
            else
                this.cb2checkedvalue.splice(index, 1);
                this.resetMessage();
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
            this.resetMessage();
            if (startDate)
                return this.storeStartDateTime;
            else
                return this.storeEndDateTime;
        },
        getTimeStampList()                          // Return as a list for timestamp
        {
            return [this.getTimeStamp(this.calendarDateTimeStart, true), this.getTimeStamp(this.calendarDateTimeEnd, false)];
        },
        resetMessage()
        {
            this.ifLoading = false;
            this.isNoEmail = false;
            this.ifSuccess = false;
            this.isInvalidDate = false;
            this.isNoSensorSelected = false;
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
        sendAndCollectData()                        // Download button function
        {
            let timelst = this.getTimeStampList();
            let currTime = new Date();

            this.isNoEmail = false;
            this.ifSuccess = false;
            this.isInvalidDate = false;
            this.ifDateWarning = false;
            this.isNoSensorSelected=false;

            if (timelst[1]-timelst[0] > 1000*60*60*24*7)
                this.ifDateWarning = true;

            if(this.selectPlatform == "Send by Email")
            {
                if(this.cb2checkedvalue.length == 0)
                    this.isNoSensorSelected = true;
                else if(this.EmailAddress == "")
                    this.isNoEmail = true;
                else if(timelst[0] > timelst[1] || timelst[1] > currTime.getTime())
                    this.isInvalidDate = true;
                else
                {
                    this.ifLoading = true;
                    this.isNoSensorSelected = false;
                    let lst = []
                    for (let val in this.cb2checkedvalue)
                    {
                        let JSONString = JSON.parse(JSON.stringify({
                            "sensor_id": this.cb2checkedvalue[val][2],
                            "param_uuid": this.cb2checkedvalue[val][1]
                        }));
                        lst.push(JSONString);
                    }
                    let query = this.url.concat("/api/reports/orgs/3/download_excel_report")
                    Sentry.captureMessage("Query Made - Download Data With Email", {extra:{query: query}});
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
                            this.APIError = "Server Error," + response["detail"]
                            Sentry.captureMessage("Download Excel API Error Message", {extra:response})
                        }
                        else
                        {
                            this.ifLoading = false;
                            this.ifSuccess = true;
                        }
                    })
                    .catch(err => {
                        this.APIError = err;
                        Sentry.captureMessage("(Fetch Function) Download Excel Error: " + err)
                    });    
                }
            }
            else if (this.selectPlatform == "Download from Widget")
            {
                if(this.checkedUnit.length == 0)
                    this.isNoSensorSelected = true;
                else if(timelst[0] > timelst[1] || timelst[1] > currTime.getTime())
                    this.isInvalidDate = true;
                else
                {
                    this.ifLoading = true;
                    let aggreFunct = "no"
                    if (this.aggregationSelect != 0)
                        aggreFunct = "average"
                    let query = this.url.concat("/api/dash_mgmt/orgs/3/dashboards/");
                    query = query.concat(String(this.dashboardID));
                    query = query.concat("/panels/");
                    query = query.concat(String(this.panelID));
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
                            Sentry.captureMessage("Setting Up Widget API Error Message", {extra:response})
                        let query = this.url.concat("/api/dash_mgmt/orgs/3/panels/");
                        query = query.concat(String(this.panelID));
                        query = query.concat("/widgets/");
                        query = query.concat(String(this.widgetID));
                        query = query.concat("/widget_instances/");
                        query = query.concat(String(this.widgetInstance));
                        Sentry.captureMessage("Query Made - Download From Widget Data", {extra:{query: query}});
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
                                this.APIError = "Server Error," + response["detail"]
                                Sentry.captureMessage("Download Widget API Error Message", {extra:response})
                            }
                            else
                            {
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
                                XLSX.writeFile(workbook, this.checkedUnit[0]+".xlsx");
                                this.ifSuccess = true;
                                this.ifLoading = false;

                            }
                        })
                        .catch(err => {
                            this.APIError = err;
                            Sentry.captureMessage("(Fetch Function) Download Widget Error: " + err)
                        });
                    })
                    .catch(err => {
                        this.APIError = err
                        Sentry.captureMessage("(Fetch Function) Setting Up Widget Error: " + err)
                    });
                }
            }
            else if (this.selectPlatform == "Download from Gateway")
            {
                this.ifLoading = true;
                if(this.checkedUnit.length == 0)
                    this.isNoSensorSelected = true;
                else if(timelst[0] > timelst[1] || timelst[1] > currTime.getTime())
                    this.isInvalidDate = true;
                else
                {
                    let data = [];
                    let column = [];
                    let page_number = this.GatewayPageNumber;
                    this.gatewayRecursiveCall(data, column, page_number, timelst);
                }
            }
        },
        gatewayRecursiveCall(data, column, page_number, timelst){
            let query = "http://127.0.0.1:8000/api/iot_mgmt/orgs/3/projects/70/gateways/390"
            // let query = this.url.concat("/api/iot_mgmt/orgs/3/projects/70/gateways/")
            // query = query.concat(String(this.checkedUnit[1]))
            query = query.concat("/data_dump_index?")
            query = query.concat("page_size=")
            query = query.concat(this.GatewayPageSize)
            query = query.concat("&page_number=")
            query = query.concat(page_number)
            query = query.concat("&to_date=")
            query = query.concat(timelst[1])
            query = query.concat("&from_date=")
            query = query.concat(timelst[0])
            // console.log(page_number);
            // Sentry.captureMessage("Query Made - Download Data from Gateway, Page " + page_number, {extra:{query: query}});
            // let eventSource = new EventSource(query);
            // eventSource.addEventListener('message', (event) => {
            //     this.sseMessage.push(event.data);
            // })
            fetch(query, {
                "method": "GET", 
                "headers": {
                    'Content-Type': 'application/json',
                    'Connection': 'keep-alive',
                    'Content-Type': 'text/event-stream',
                    'Authorization' : "Bearer ".concat(this.db_accessToken)
                }
            })
            .then(response => {
                const reader = response.body.getReader();
                const decoder = new TextDecoder();
                
                const processStream = () => {
                    return reader.read().then(({done, value}) => {
                        if(done){
                            console.log("done")
                            return;
                        }
                        console.log(decoder.decode(value));
                        return processStream();
                    });
                };
                return processStream();
            })
            // .then(response => {
            //     // if ("detail" in response)
            //     //     Sentry.captureMessage("Download Gateway API Error Message", {extra:response})
            //     console.log(response)
            //     // for (let dd_value in response["data_dumps"])
            //     // {
            //     //     let temp_dd_value = [];
            //     //     if(dd_value == 0 && data.length == 0)
            //     //     {
            //     //         column.push("data")
            //     //         column.push("timestamp");
            //     //     //     column.push("date (GMT 8+)");
            //     //     //     column.push("time_stamp");
            //     //     //     for (let dd2_value in response["data_dumps"][dd_value]["data"]["assets_params"])
            //     //     //         column.push(dd2_value);
            //     //     }
            //     //     // let tempDate = response["data_dumps"][dd_value]["data"]["timestamp"] * 1000;
            //     //     // temp_dd_value.push((new Date(tempDate)).toLocaleString());
            //     //     temp_dd_value.push(JSON.stringify(response["data_dumps"][dd_value]["data"]));
            //     //     temp_dd_value.push(response["data_dumps"][dd_value]["data"]["timestamp"]);
            //     //     // for (let dd2_value in response["data_dumps"][dd_value]["data"]["assets_params"])
            //     //     //         temp_dd_value.push(response["data_dumps"][dd_value]["data"]["assets_params"][dd2_value]);
            //     //     data.push(temp_dd_value);
            //     //     // console.log(data);
            //     // }
            //     // // console.log(response["data_dumps"].length);
            //     // if (response["data_dumps"].length == this.GatewayPageSize)
            //     // {
            //     //     // console.log(data, column)
            //     //     this.GatewayPageNumber = this.GatewayPageNumber + 1;
            //     //     this.gatewayRecursiveCall(data, column, page_number+1, timelst);
            //     // }
            //     // else
            //     // {
            //     //     data.push(column);
            //     //     // data.push(["data"])
            //     //     this.GatewayPageNumber = 1;
            //     //     let workbook = XLSX.utils.book_new(),
            //     //     worksheet = XLSX.utils.aoa_to_sheet(data.reverse());
            //     //     workbook.SheetNames.push("Generated Data");
            //     //     workbook.Sheets["Generated Data"] = worksheet;
            //     //     XLSX.writeFile(workbook, this.checkedUnit[0]+".xlsx");
            //     //     this.ifSuccess = true;
            //     //     this.ifLoading = false;
            //     // }
            // })
            .catch(err => {
                this.APIError = err;
                // Sentry.captureMessage("(Fetch Function) Download Gateway Error: " + err)
            });
        },
        sendEmailSelection()                            // Query for list of units and senors
        {
            let query = this.url.concat("/api/reports/orgs/3/entities")
            Sentry.captureMessage("Query Made - Get Sensors List for Email", {extra:{query: query}});
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
                    Sentry.captureMessage("Retrieve Dashboard List API Error Message", {extra:response})
                this.isDDLDisabled = false;
                this.db_entitiesSelector = response["entities"][0]["entities"]
            })
            .catch(err => {
                this.APIError = err;
                Sentry.captureMessage("(Fetch Function) Retrieve Dashboard List Error: " + err)
            });    
        },
        sendGatewaySelection()                          // Query for list of gateways
        {
            this.isDDLDisabled=true;
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
                    Sentry.captureMessage("Retrieve Gateway List API Error Message", {extra:response})
                this.isDDLDisabled = false;
                this.db_entitiesSelector = response["gateways"];
            })
            .catch(err => {
                this.APIError = err;
                Sentry.captureMessage("(Fetch Function) Retrieve Gateway List Error" + err)
            });
        },
        sendWidgetSelectionLayer1()                          // Query for Dashboard
        {
            this.isDDLDisabled=true;
            let query = this.url.concat("/api/dash_mgmt/orgs/3/dashboards?")
            query = query.concat("page_size=1000&from=0")
            Sentry.captureMessage("Query Made - Get Dashboard List", {extra:{query: query}});
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
                    Sentry.captureMessage("Retrieve Widget List API Error Message", {extra:response});
                let temp_entities = [];
                for (let lstIndex in response["dashboards"])
                {
                    let name = response["dashboards"][lstIndex]["name"].toUpperCase();
                    if (name.includes("SERVICE") || name.includes("ANALYSIS") || name.includes("DEBUG"))
                    {
                        temp_entities.push(response["dashboards"][lstIndex]);
                    }
                }
                this.db_entitiesSelector = temp_entities;
                this.isDDLDisabled = false;
            })
            .catch(err => {
                this.APIError = err;
                Sentry.captureMessage("(Fetch Function) Retrieve Widget List Error: " + err);
            });
        },
    }
}
</script>

<template>
    <div v-if="adBlockerActive">
        <p>Please disable your ad blocker to continue using this website.</p>
    </div>
    <div v-else>
        <div class="DR-mainContainer">
            <div class="DR-tree-Container">
                <div class="cbLayer1Container">
                    <div class="titleText1">Select Sensors or Device</div>
                    <div class="cb1group" v-for="val in db_entitiesSelector" :key="val.name+val.id">
                        <div class="cb1group1">
                            <input class="cb1rb" type ="radio" :id="val.name+val.id" name="selectUnit" v-model="checkedUnit" :value="getNameID(val.name, val.id)" @click="cb1uncheck(getNameID(val.name, val.id))"/>
                            <label class="cb1lbl" :for="val.name+val.id">{{ val.name }}</label>
                        </div>
                        <div class="cbLayer2Container" v-if="checkedUnit[0]+checkedUnit[1] === val.name+val.id">
                            <div class="cb2group" v-for="val2 in extractData(val)">
                                <div v-if="selectPlatform == 'Send by Email'">
                                    <div class="cb2group1" v-if="!('entities' in val2)">
                                        <input class="cb2cb" type="checkbox" :id="val2.name+val2.id" :value="val2.name+val2.id" watch:checked="cb2ischecked" @click="cb2storeValue(getNameID(val2.name, val2.uuid, val.id))"/>
                                        <label class="cb2lbl" :for="val2.name+val2.id">{{ val2.name }}</label>
                                    </div>
                                    <div class="cb2group1" v-else>
                                        <input class="cb3rb" type ="radio" :id="val2.name+val2.id" name="selectUnit2" v-model="checkedUnitSub" :value="getNameID(val2.name, val2.id)" @click="cb2uncheck(getNameID(val2.name, val2.id))"/>
                                        <label class="cb3lbl" :for="val2.name+val2.id">{{ val2.name }}</label>
                                    </div>
                                    <div class="cbLayer2Container" v-if="checkedUnitSub[0]+checkedUnitSub[1] === val2.name+val2.id && selectPlatform == 'Send by Email'">
                                        <div class="cb3group" v-for="val3 in val2['entities']">
                                            <div class="cb2group1">
                                                <input class="cb3cb" type="checkbox" :id="val3.name+val3.id" :value="val3.name+val3.id" watch:checked="cb2ischecked" @click="cb2storeValue(getNameID(val3.name, val3.uuid, val2.id))"/>
                                                <label class="cb3lbl" :for="val3.name+val3.id">{{ val3.name }}</label>
                                            </div>
                                        </div>
                                    </div>
                                </div>
                            </div>
                        </div>
                    </div>
                </div>
            </div>
            <div class="column2">
                <div class="DatePickerContainer">
                    <div class="DatePickerContainer2">
                        <label class="titleText1">Select Date:</label>
                        <div class="DatePickerContainer3">
                            <label class="DPlbl2">Start Date:</label>
                            <VueCtkDateTimePicker class="calendarPicker" v-model="calendarDateTimeStart" />
                        </div>
                        <div class="DatePickerContainer3">
                            <label class="DPlbl2">End Date:</label>
                            <VueCtkDateTimePicker class="calendarPicker" v-model="calendarDateTimeEnd" />
                        </div>
                    </div>
                </div>
                <div class="DatePickerContainer">
                    <div class="DatePickerContainer2">
                        <div class="titleText1">Summary:</div>
                        <div class="selectedUnit">Selected Unit:</div>
                        <div class="selectUnitVal" v-if="checkedUnit.length>0">{{ checkedUnit[0] }}</div>
                        <div class="selectUnitVal1" v-if="checkedUnitSub.length>0">&#x1F872; {{ checkedUnitSub[0] }}</div>
                        <div class="selectedUnit">Selected Sensor:</div>
                        <div class="selectUnitVal" v-for="val in cb2checkedvalue">{{ val[0] }}</div>
                        <div class="selectedUnit">Selected Sending Platform:</div>
                        <div class="selectUnitVal" v-if="selectPlatform != null">{{ selectPlatform }}</div>
                    </div>
                </div>
                <div class="DatePickerContainer">
                    <div class="DatePickerContainer2">
                        <!-- <div class="submitContainer"> -->
                        <select class="pltSelect" v-model="selectPlatform" :disabled="isDDLDisabled" @change="clearSubEntity()">
                            <option value="Send by Email">Send by Email</option>
                            <option value="Download from Widget">Download from Widget</option>
                            <option value="Download from Gateway">Download from Gateway</option>
                        </select>
                        <div v-if="ifDateWarning" class="WarningsMsg">(!) Your selected date range is more than a week, this may cause delays or failure in data retrieval.</div>
                        <div class="Msg1" v-if="ifLoading && this.selectPlatform != 'Download from Gateway'">Communicating with server.</div>
                        <div class="Msg1" v-if="ifLoading && this.selectPlatform == 'Download from Gateway'">Communicating with server, page {{ GatewayPageNumber }}...</div>
                        <div class="errorMsg1" v-if="isNoSensorSelected">Please select sensors!</div>
                        <div class="errorMsg1" v-if="isNoEmail">Please enter a valid E-mail!</div>
                        <div class="errorMsg1" v-if="isInvalidDate">Please ensure that you have selected the correct date range!</div>
                        <div class="errorMsg1" v-if="APIError.length>0">{{ APIError }}</div>
                        <div class="successMsg1" v-if="ifSuccess">Parameters query success.</div>
                        <div v-if="this.selectPlatform == 'Send by Email'">
                            <div class="DPlbl2">Email Address:</div>
                            <input class="inputClass" type="text" v-model="EmailAddress"/>
                        </div>
                        <div v-else-if="this.selectPlatform == 'Download from Widget'">
                            <div v-if="ifWarningMsg" class="WidgetInstructions" @click="ifWarningMsg= !ifWarningMsg">The device selection will display a list of service dashboard. If the selected dashboard do not allow you to download data, then the dashboard do not have an existing widget for you to download data from.<br><br> How this function works is that the interface will search for a panel tab named "DATA RETRIEVAL". The program will only read all parameters from the first widget added into "DATA RETRIEVAL" panel tab.</div>
                            <div v-else class="WidgetInstructions" @click="ifWarningMsg= !ifWarningMsg">Instructions...</div>
                            <div class="DPlbl2">Aggregation:</div>
                            <select class="agreSelect" v-model="aggregationSelect" :disabled="isDDLDisabled">
                                <option value="0">No</option>
                                <option value="5">5 minutes</option>
                                <option value="30">30 minutes</option>
                            </select>
                        </div>
                        <button class="submitbtn" :disabled="isDownloadDisabled" @click="sendAndCollectData()">Build Data/Receive Email</button>
                    </div>
                </div>
                <!-- <div v-for="val in sseMessage">
                    {{ val }}
                </div> -->
            </div>
        </div>
    </div>
</template>

<style scoped>
.DR-mainContainer{
    display: flex;
    flex-direction: row;
}
.column2{
    /* border: solid; */
    width: 50%;
}
.DR-tree-Container{
    /* border: solid; */
    width: 50%;
    padding: 1.5rem;
}
.titleText1{
    font-size: 2rem;
}
.cb1group{
    /* border: solid; */
    display: flex;
    flex-direction: column;    
    /* justify-content: space-between; */
}
.cb1group1
{
    display: flex;
    gap: 1rem;
}
.cbLayer1Container{
    border: solid;
    padding: 0.5rem 1.5rem;
    border-radius: 2rem;
}
.cb1rb{
    /* width: 1.1rem; */
    cursor: pointer;
}
.cb1lbl{
    font-size: 1.1rem;
    cursor: pointer;
}
.cbLayer2Container{
    /* border: solid; */
    display: flex;
    flex-direction: column;
    padding: 0 2rem;
}
.cb2group1
{
    display: flex;
    gap: 1rem;
}
.DatePickerContainer{
    display: flex;
    flex-direction: column;
    padding: 1.5rem;
    gap: 1rem;
    /* width: 100%; */
}
.DatePickerContainer2{
    border: solid;
    border-radius: 1rem;
    padding: 0.5rem 1.5rem;
}
.DPlbl
{
    font-size: 1.2rem;
}
.dateContainer{
    border: solid;
}
.summaryContainer{
    border: solid;
    border-radius: 1rem;
}
.calendarPicker{
    border: solid;
    min-width: 4rem;
}
.selectedUnit{
    font-size: 1.1rem;
    font-weight: bold;
}
.selectUnitVal{
    padding-left: 1.5rem;
    border-bottom: solid;
    border-radius: 0.5rem;
    border-width: 0.1rem;
}
.selectUnitVal1{
    padding-left: 2rem;
    border-bottom: solid;
    border-radius: 0.5rem;
    border-width: 0.1rem;
}
.submitContainer{
    display: flex;
}
.submitbtn
{
    width: 100%;
    height: 3rem;
    font-size: 1.3rem;
    border-radius: 1rem;
}
.pltSelect{
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
.cb3group
{
    display: flex;
    gap: 1rem;
}
.Msg1
{
    font-size: 1.2rem;
    text-align: center;
    margin-bottom: 1rem;
    text-decoration: underline;
    text-underline-offset: 0.5rem;
}
.errorMsg1
{
    color: rgb(230, 85, 85);
    font-size: 1.2rem;
    text-align: center;
    margin-bottom: 1rem;
    text-decoration: underline;
    text-underline-offset: 0.5rem;
}
.successMsg1
{
    color: rgb(114, 216, 136);
    font-size: 1.2rem;
    text-align: center;
    margin-bottom: 1rem;
    text-decoration: underline;
    text-underline-offset: 0.5rem;
}
.inputClass
{
    width: 100%;
    margin-bottom: 1rem;
    margin-top: 0.2rem;
    border-width: 0.1rem;
    border-radius: 0.5rem;
    padding: 0.5rem 1rem;
    font-size: 1.2rem;
    height: 2rem;
}
.WidgetInstructions
{
    display: flex;
    text-align: justify;
    margin-bottom: 1rem;
    color: rgb(189, 140, 34);
    cursor: pointer;
}
.WarningsMsg
{
    display: flex;
    text-align: justify;
    margin-bottom: 1rem;
    color: rgb(236, 127, 37);
}
@media (max-width: 800px) {
    .DR-tree-Container{
        width: 100%;
    }
    .DR-mainContainer{
        flex-direction: column;
    }
    .column2{
        width: 100%;
    }
}
</style>