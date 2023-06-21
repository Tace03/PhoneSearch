//////////////////////////////////////////////////////////////////////////////////
// Author: Naina Gupta
// July'2021 - ModbusTCPLogger v1.0
//////////////////////////////////////////////////////////////////////////////////

#include "battery_json.h"

//Parameter Mapping
//prcr -> primary_charging_relay
//prdr -> primary_discharge_relay
//prpp -> primary_positive_pump
//prnp -> primary_negative_pump

//sm -> system_mode // Added on 11 Aug 21
//sas -> system_alarm_status // Added on 11 Aug 21

//bv -> balancing_valve
//pv -> positive_valve
//nv -> negative_valve
//soc -> state_of_charge
//bvolt -> bcu_voltage
//bcurr -> bcu_current
//bpow -> bcu_power
//bsoc -> bcu_state_of_charge

//bcmst -> bcu_mode_status // Added on 11 Aug 21
//hs -> bcu_hydrogen_sensor // Added on 11 Aug 21
//ls -> bcu_leakage_sensor // Added on 11 Aug 21

//ss -> smoke_sensor
//bocv -> bcu_ocv
//bptt -> bcu_positive_tank_temp
//bntt -> bcu_negative_tank_temp
//pthlf -> positive_tank_high_level_float
//nthlf -> negative_tank_high_level_float
//ptllf -> positive_tank_low_level_float
//ntllf -> negative_tank_low_level_float
//prvolt -> primary_stack_voltage
//prcurr -> primary_stack_current
//prspps -> primary_stack_positive_pressure_sensor
//prsnps -> primary_stack_negative_pressure_sensor
//pspd -> positive_stack_pressure_delta // modified on 11 Aug 21
//pspd1 -> b1_primary_stack_pressure_delta // Added on 11 Aug 21
//temp -> sensor_temp
//hum -> humiditiy

//p1dcv -> pcs1_dc_volts // Added on 11 Aug 21
//p1dbc -> pcs1_dc_batt_current // Added on 11 Aug 21
//p1invpow -> pcs1_dc_inverter_power // Added on 11 Aug 21

//pcvolt -> pcs1_voltage
//pccurr -> pcs1_current
//pcrpow -> pcs1_reactive_power
//pclpow -> pcs1_load_power
//pcacpow -> pcs1_ac_supply_power

//p1acost -> pcs1_ac_out_status // Added on 11 Aug 21
//p1fst -> pcs1_fault_status // Added on 11 Aug 21
//p1fsp -> pcs1_fan_speed // Added on 11 Aug 21

//sespps -> primary_stack_positive_pressure_sensor
//sesnps -> primary_stack_negative_pressure_sensor

//{ "assets_params": { "prpp": 1, "pv": 0, "bptt": 38.0599975585938, "ss": 0, "pcvolt": 234.929992675781, "soc": 25.7399997711182, "hum": 53.6999969482422, "prnp": 1, "pcrpow": -90.1599960327148, "bsoc": 25.7399997711182, "prcr": 1, "prvolt": 48.7870025634766, "pclpow": 185.06999206543, "prspps": 231, "bocv": 1.32700002193451, "prcurr": 4.75999975204468, "nthlf": 1, "asset_id": 2, "bntt": 38.3299980163574, "nv": 0, "bpow": 0.229999989271164, "bcurr": 4.75999975204468, "pthlf": 1, "bvolt": 48.8030014038086, "pccurr": -0.949999988079071, "ptllf": 0, "temp": 34.8999977111816, "pspd": 39.2399978637695, "prdr": 1, "bv": 0, "ntllf": 0, "pcacpow": 0, "prsnps": 270.239990234375 }, "gateway_id": "69", "org_id": "3", "project_id": "70", "timestamp": 1628679769, "uptime": 13543 }

//	bcu1Battery000PressureDelta0 


float get_charge_level(float soc)
{
	float cl = (soc - 10) * 100/78.5;
	if (cl < 0)
		cl = 0;
	if (cl > 100)
		cl = 100;
	printf("Charging level %f", cl);
	return cl;
}

float state_of_charge_config(float soc)
{
	float sc = (soc*100)/88.5;
	if (sc < 0)
		sc = 0;
	if (sc > 100)
		sc = 100;
	return sc;
}




void encode_sensor_data_to_json(json& out_data, const sensor_data& inp_data, system_config& sys_config)
{
	out_data["timestamp"] = inp_data.timestamp;
	out_data["uptime"] = inp_data.uptime;

	out_data["assets_params"]["asset_id"] = sys_config.asset_id;
	
	out_data["assets_params"]["prcr"] = inp_data.battery.primary_charging_relay;
	out_data["assets_params"]["prdr"] = inp_data.battery.primary_discharge_relay;
	out_data["assets_params"]["prpp"] = inp_data.battery.primary_positive_pump;
	out_data["assets_params"]["prnp"] = inp_data.battery.primary_negative_pump;

	out_data["assets_params"]["sm"] = inp_data.battery.system_mode; // Added on 11 Aug 21
	out_data["assets_params"]["sas"] = inp_data.battery.system_alarm_status; // Added on 11 Aug 21

    out_data["assets_params"]["bv"] = inp_data.battery.balancing_valve;
	out_data["assets_params"]["pv"] = inp_data.battery.positive_valve;
    out_data["assets_params"]["nv"] = inp_data.battery.negative_valve;
	out_data["assets_params"]["soc"] = (float)inp_data.battery.state_of_charge;
//	out_data["assets_params"]["soc"] = state_of_charge_config(get_charge_level((float)inp_data.battery.state_of_charge));
	out_data["assets_params"]["bcmst"] = inp_data.battery.bcu_mode_status; // Added on 11 Aug 21
	out_data["assets_params"]["hs"] = (float)inp_data.battery.bcu_hydrogen_sensor; // Added on 11 Aug 21
	out_data["assets_params"]["ls"] = inp_data.battery.bcu_leakage_sensor; // Added on 11 Aug 21
   out_data["assets_params"]["ls2"] = inp_data.battery.bcu_leakage_sensor2;

    out_data["assets_params"]["bvolt"] = (float)inp_data.battery.bcu_voltage;
	out_data["assets_params"]["bcurr"] = (float)inp_data.battery.bcu_current;
    out_data["assets_params"]["bpow"] = (float)inp_data.battery.bcu_power;

	out_data["assets_params"]["bsoc"] = (float)inp_data.battery.bcu_state_of_charge;
    out_data["assets_params"]["ss"] = inp_data.battery.smoke_sensor;
	out_data["assets_params"]["bocv"] = (float)inp_data.battery.bcu_ocv;
    out_data["assets_params"]["bptt"] = (float)inp_data.battery.bcu_positive_tank_temp;
	out_data["assets_params"]["bntt"] = (float)inp_data.battery.bcu_negative_tank_temp;
    out_data["assets_params"]["pthlf"] = inp_data.battery.positive_tank_high_level_float;
	out_data["assets_params"]["nthlf"] = inp_data.battery.negative_tank_high_level_float;
    out_data["assets_params"]["ptllf"] = inp_data.battery.positive_tank_low_level_float;
	out_data["assets_params"]["ntllf"] = inp_data.battery.negative_tank_low_level_float;
    out_data["assets_params"]["prvolt"] = (float)inp_data.battery.primary_stack_voltage;
	out_data["assets_params"]["prcurr"] = (float)inp_data.battery.primary_stack_current;
    out_data["assets_params"]["prspps"] = (float)inp_data.battery.primary_stack_positive_pressure_sensor;
	out_data["assets_params"]["prsnps"] = (float)inp_data.battery.primary_stack_negative_pressure_sensor;
    out_data["assets_params"]["pspd"] = (float)inp_data.battery.positive_stack_pressure_delta; //Modified on 11 Aug 21
	out_data["assets_params"]["pspd1"] = (float)inp_data.battery.b1_primary_stack_pressure_delta; //Added on 11 Aug 21
	out_data["assets_params"]["temp"] = (float)inp_data.battery.sensor_temp;
    out_data["assets_params"]["hum"] = (float)inp_data.battery.humidity;
	
	out_data["assets_params"]["p1dcv"] = (float)inp_data.battery.pcs1_dc_volts; // Added on 11 Aug 21
	out_data["assets_params"]["p1dbc"] = (float)inp_data.battery.pcs1_dc_batt_current; // Added on 11 Aug 21
	out_data["assets_params"]["p1invpow"] = (float)inp_data.battery.pcs1_dc_inverter_power; // Added on 11 Aug 21

	out_data["assets_params"]["pcvolt"] = (float)inp_data.battery.pcs1_voltage;
    out_data["assets_params"]["pccurr"] = (float)inp_data.battery.pcs1_current;
	out_data["assets_params"]["pcrpow"] = (float)inp_data.battery.pcs1_reactive_power;
    out_data["assets_params"]["pclpow"] = (float)inp_data.battery.pcs1_load_power;
	out_data["assets_params"]["pcacpow"] = (float)inp_data.battery.pcs1_ac_supply_power;
	out_data["assets_params"]["p1acpow"] = (float)inp_data.battery.pcs1_ac_power;

	out_data["assets_params"]["p1acost"] = (float)inp_data.battery.pcs1_ac_out_status;  // Added on 11 Aug 21
	out_data["assets_params"]["p1fst"] = (float)inp_data.battery.pcs1_fault_status;  // Added on 11 Aug 21
	out_data["assets_params"]["p1fsp"] = (float)inp_data.battery.pcs1_fan_speed;  // Added on 11 Aug 21

	out_data["assets_params"]["charge_level"] = get_charge_level((float)inp_data.battery.state_of_charge); // Added on 16 Aug 21

	out_data["assets_params"]["PVStat"] = (float)inp_data.battery.system0PVEnable;
    out_data["assets_params"]["PVBP"] = (float)inp_data.battery.system0PVChargePower;
	out_data["assets_params"]["PVSP"] = (float)inp_data.battery.system0PVTotalPower;
    out_data["assets_params"]["Freq"] = (float)inp_data.battery.pcs1InvFreq;
	out_data["assets_params"]["InvTemp"] = (float)inp_data.battery.pcs1InternalTemperature;
 
 	out_data["assets_params"]["sespps"] = (float)inp_data.battery.secondary_stack_positive_pressure_sensor;
	out_data["assets_params"]["sesnps"] = (float)inp_data.battery.secondary_stack_negative_pressure_sensor;
  out_data["assets_params"]["sevolt"] = (float)inp_data.battery.secondary_stack_voltage;
	out_data["assets_params"]["securr"] = (float)inp_data.battery.secondary_stack_current;
 
 out_data["assets_params"]["dcdb"] = inp_data.battery.dc_main_contactor;
	out_data["assets_params"]["pcr "] = inp_data.battery.pre_charge_relay;
  out_data["assets_params"]["acfanc"] = inp_data.battery.ac_fan_coil1;
	out_data["assets_params"]["ffan"] = inp_data.battery.fluid_fan;
 out_data["assets_params"]["dcfan"] = inp_data.battery.dcdb_fan;
	out_data["assets_params"]["stk1"] = inp_data.battery.stk_contactor1;
	out_data["assets_params"]["stk2"] = inp_data.battery.stk_contactor2;


	//PCS2 
	out_data["assets_params"]["p2dcv"] = (float)inp_data.battery.pcs2_dc_volts;
	out_data["assets_params"]["p2dbc"] = (float)inp_data.battery.pcs2_dc_batt_current;
	out_data["assets_params"]["p2invpow"] = (float)inp_data.battery.pcs2_dc_inverter_power;

	out_data["assets_params"]["pc2volt"] = (float)inp_data.battery.pcs2_voltage;
    	out_data["assets_params"]["pc2curr"] = (float)inp_data.battery.pcs2_current;
	out_data["assets_params"]["pc2rpow"] = (float)inp_data.battery.pcs2_reactive_power;
    	out_data["assets_params"]["pc2lpow"] = (float)inp_data.battery.pcs2_load_power;
	out_data["assets_params"]["pc2acpow"] = (float)inp_data.battery.pcs2_ac_supply_power;
	out_data["assets_params"]["p2acpow"] = (float)inp_data.battery.pcs2_ac_power;

	out_data["assets_params"]["p2acost"] = (float)inp_data.battery.pcs2_ac_out_status;
	out_data["assets_params"]["p2fst"] = (float)inp_data.battery.pcs2_fault_status;
	out_data["assets_params"]["p2fsp"] = (float)inp_data.battery.pcs2_fan_speed;

    	out_data["assets_params"]["p2Freq"] = (float)inp_data.battery.pcs2InvFreq;
	out_data["assets_params"]["p2InvTemp"] = (float)inp_data.battery.pcs2InternalTemperature;


	//PCS3
	out_data["assets_params"]["p3dcv"] = (float)inp_data.battery.pcs3_dc_volts;
	out_data["assets_params"]["p3dbc"] = (float)inp_data.battery.pcs3_dc_batt_current;
	out_data["assets_params"]["p3invpow"] = (float)inp_data.battery.pcs3_dc_inverter_power;

	out_data["assets_params"]["pc3volt"] = (float)inp_data.battery.pcs3_voltage;
	out_data["assets_params"]["pc3curr"] = (float)inp_data.battery.pcs3_current;
	out_data["assets_params"]["pc3rpow"] = (float)inp_data.battery.pcs3_reactive_power;
    	out_data["assets_params"]["pc3lpow"] = (float)inp_data.battery.pcs3_load_power;
	out_data["assets_params"]["pc3acpow"] = (float)inp_data.battery.pcs3_ac_supply_power;
	out_data["assets_params"]["p3acpow"] = (float)inp_data.battery.pcs3_ac_power;

	out_data["assets_params"]["p3acost"] = (float)inp_data.battery.pcs3_ac_out_status;
	out_data["assets_params"]["p3fst"] = (float)inp_data.battery.pcs3_fault_status;
	out_data["assets_params"]["p3fsp"] = (float)inp_data.battery.pcs3_fan_speed;

    	out_data["assets_params"]["p3Freq"] = (float)inp_data.battery.pcs3InvFreq;
	out_data["assets_params"]["p3InvTemp"] = (float)inp_data.battery.pcs3InternalTemperature;
	
 
}
