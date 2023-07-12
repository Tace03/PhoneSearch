<script>
import { isProxy, toRaw } from 'vue';
export default {
    name: "RecursiveTreeComponent",
    data() {
        return {
            isCBUnitChecked: [],
            isRBUnitChecked: [],
            unitCheckedValue: [],
            queryItems: [],
        }
    },
    components: {
        RecursiveTreeComponent: () => import('@/components/interfaces/dataRetrieve/treeComponent.vue')
    },
    watch: {
        'selectionType': function () {
            this.isRBUnitChecked = [];
        }
    },
    props: {
        items: {
            type: Array,
            default: () => []
        },
        selectionType: {
            type: String,
            default: ""
        },
        isDownloadDisabled: {
            type: Boolean,
            default: false
        }
    },
    methods: {
        printSomeVal(val) {
            console.log(val)
        },
        lstToString(lst) {
            let rtnVal = "";
            for (let lstVal of lst)
                rtnVal += lstVal;
            return rtnVal
        },
        extractData(val) {                          // Used to extract data to raw and retrieve entities
            if (isProxy(val))
                return toRaw(val)
            else
                return val
        },
        storeCheckedUnitValue(lst) {                // Store checkbox value if does not exist and remove if exist
            let index = -1;
            for (let value in this.unitCheckedValue)
            {
                if (lst[0]==this.unitCheckedValue[value][0] && lst[1]==this.unitCheckedValue[value][1])
                {
                    index = value;
                    break;
                }
            }
            if (index == -1)
                this.unitCheckedValue.push(lst);
            else
                this.unitCheckedValue.splice(index, 1);
            this.$emit('update-queryItem', this.unitCheckedValue)
        },
        rbCheckUncheck(val){
            let tempVal = this.extractData(this.isRBUnitChecked);
            if (tempVal.length > 0)
            {
                if (this.lstToString(val) === this.lstToString(tempVal))
                {
                    this.isRBUnitChecked = [];
                    if(this.selectionType !== "Send by Email")
                        this.$emit('update-queryItem', []);
                }
                else
                {
                    this.queryItems = [...val];
                    this.$emit('update-queryItem', val);
                }
            }
            else
            {
                this.queryItems = [...val];
                this.$emit('update-queryItem', val);
            }
        },
        handleMessageFromChild(message) {
            if ( this.queryItems[this.queryItems.length-1].length > 1)
                this.queryItems.pop();
            this.queryItems.push(this.extractData(message));
            this.$emit('update-queryItem', this.queryItems)
        }
    },
};
</script>

<template>
    <div class="TreeSubLayerContainer">
        <div class="TreeSubLayerGroupMain" v-for="item in extractData(items)">
            <div class="TreeSubLayerGroup" v-if="!('entities' in item) && selectionType === 'Send by Email'">
                <input class="checkboxInput" type="checkbox" :id="item.name+item.id" :value="['cb', item.name, item.id]" :disabled="isDownloadDisabled"
                    watch:checked="isCBUnitChecked" @click="storeCheckedUnitValue(['cb', item.name, item.uuid])"/>
                <label class="checkboxlabel" :for="item.name+item.id">{{ item.name }}</label>
            </div>
            <div class="TreeSubLayerGroup" v-else>
                <input class="checkboxInput" v-if="selectionType === 'Send by Email'" :disabled="isDownloadDisabled"
                    type="radio" :id="item.name+item.id" :value="['rb', item.name, item.id]"
                    v-model="isRBUnitChecked" @click="rbCheckUncheck(['rb', item.name, item.id])"/>
                <input class="checkboxInput" v-else :disabled="isDownloadDisabled"
                    type="radio" :id="item.name+item.id" :value="['cb', item.name, item.id]"
                    v-model="isRBUnitChecked" @click="rbCheckUncheck(['cb', item.name, item.id])"/>
                <label class="checkboxlabel" :for="item.name+item.id">{{ item.name }}</label>
            </div>
            <div class="cbLayer2Container" v-if="lstToString(extractData(isRBUnitChecked)) === 'rb'+item.name+item.id">
                    <RecursiveTreeComponent :isDownloadDisabled="isDownloadDisabled" :selectionType="selectionType" :items="item['entities']" @update-queryItem="handleMessageFromChild"/>
            </div>
        </div>
    </div>
</template>

<style scoped>
.TreeSubLayerContainer {
    font-size: 1.1rem;
    padding-left: 1.2rem;
}
.TreeSubLayerGroup {
    gap: 0.5rem;
    display: flex;
}
</style>