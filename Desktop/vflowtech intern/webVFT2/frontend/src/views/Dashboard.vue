<template>
    <!-- <div class="my-4">
        <button @click="this.$router.push({ name: 'dashList', params: { id: this.$route.params.org_id } })">Back</button>
    </div> -->

    <div class="" v-if="responses">
        <v-card color="primary">
            <v-tabs v-model="tab" align-tabs="title">
                <v-tab v-for="panel in responses.panels" :key="panel.id" :value="panel.id">
                    {{ panel.id }}
                </v-tab>
            </v-tabs>

            <v-window v-model="tab">
            <v-window-item v-for="panel in responses.panels" :key="panel.id" :value="panel.id">
                    <div class="col mb-4">
                    <div class="card"  v-for="res in getPanelData(panel.id)" :key="res.id" @click="toggle(res.id)">
                            <div class="card-body">
                                <!-- // <div v-if="res.widget_category[1].includes('gauge')">
                                //     <apexchart type="radialBar" height="350" :series="[{data: res.series[0].data}]"></apexchart>
                                // </div> -->
                                <div v-if="res.widget_category[1]=='line_symbol'">
                                    <apexchart type="line" height="350" :options="options.line" :series="[{data: res.series[0].data}]"></apexchart>
                                </div>
                                <div v-else>
                                    <p class="text-dark">{{ res.id }}</p>
                                    <p class="text-dark" v-if="toggles[res.id]">{{ res }}</p>
                                </div>
                            </div>
                    </div>
                    </div>
            </v-window-item>
            </v-window>
        </v-card>
    </div>
    <p v-else>Loading...</p>
</template>

<script>
import FetchService from '../services/fetch-service';

export default {
    data() {
        return {
            tab: null,
            panels: {},
            responses: null,
            realtime: {},
            intervals: [],
            toggles: {},
            options: {
                line: {
                    xaxis: {
                        type: 'datetime'
                    }
                },
                gauge: {
                    plotOptions: {
                        radialBar: {
                            startAngle: -135,
                            endAngle: 135,
                            track: {
                                background: '#333',
                                startAngle: -135,
                                endAngle: 135,
                            },
                            dataLabels: {
                                name: {
                                show: false,
                                },
                                value: {
                                fontSize: "30px",
                                show: true
                                }
                            }
                        }
                    }
                }
            }
        }
    },
    methods: {
        toggle(id) {
            this.toggles[id] = !this.toggles[id];
        },
        getPanel(id) {
            if (id in this.panels) {
                return this.panels[id];
            } else {
                FetchService.fetch_panel(this.$route.params.org_id, this.$route.params.dash_id, id).then(
                    response => {
                        this.panels[id] = response.data;
                        this.realtime[id] = response.realtime;
                    })
            }
            return this.panels[id];
        },
    },
    mounted() {
        FetchService.fetch_dash(this.$route.params.org_id, this.$route.params.dash_id).then(
            data => {
                this.responses = data;
            }
        )
    },
    computed: {
        getPanelData() {
            return (id) => {
            return this.panels[id];
            };
        },
    },
    beforeUnmount() {
    window.removeEventListener('beforeunload', () => {
        this.intervals.forEach(interval => clearInterval(interval));
        this.intervals = [];});
    },
    watch: {
        tab(newTab, oldTab) {
            if (newTab !== oldTab) {
                this.getPanel(newTab);
            }
        },
        realtime: {
            handler(newRealdata, oldRealdata) {
                this.intervals.forEach(interval => clearInterval(interval));
                this.intervals = [];
                newRealdata[this.tab].map((widget) => {
                        const interval = setInterval(() => {
                        FetchService.fetch_widget(this.$route.params.org_id, this.$route.params.dash_id, this.tab, widget.widget_id, widget.id).then(
                            res => {
                                this.panels[widget.id] = res;
                            }
                        )
                    }, 15000);
                this.intervals.push(interval);
                })
            },
            deep:true,
        },
    }
}
</script>

<style></style>