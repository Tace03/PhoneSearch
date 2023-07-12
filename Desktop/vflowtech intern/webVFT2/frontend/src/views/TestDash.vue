<template>
    <button @click="this.$router.push({ name: 'dashList', params: { id: this.$route.params.org_id } })">Back</button>
    <div v-if="responses">
        <ul>
            <li v-for="res in responses" :key="res.id" @click="toggle(res.id)">
                {{ res.id }}
                <p v-if="toggles[res.id]">{{ res }}</p>
                <div v-if="res.widget_category[1]=='line_symbol'">
                    <apexchart type="line" height="350" :options="options.line" :series="[{data: res.series[0].data}]"></apexchart>
                </div>
                <div v-if="res.widget_category[1].includes('gauge')">
                    <apexchart type="radialBar" height="350" :options="options.gauge" :series="[{data: res.series[0].data}]"></apexchart>
                </div>
            </li> 
        </ul>
    </div>
    <p v-else>Loading...</p>
</template>

<script>
import FetchService from '../services/fetch-service';
export default {
    data() {
        return {
            responses: null,
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
        }
    },
    mounted() {
        FetchService.fetch_dash(this.$route.params.org_id, this.$route.params.dash_id).then(
            data => {
                this.responses = data;
            }
        )
    }
}
</script>

<style></style>