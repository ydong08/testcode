
CREATE DATABASE IF NOT EXISTS acmesh;
use acmesh;


CREATE TABLE `ac_common_info` (
`ac_mac`  char(6)  NOT NULL DEFAULT '1A2B3C' COMMENT 'AC的MAC地址，16进制表示' ,
`ac_max_controller_num`  smallint(6) NOT NULL DEFAULT 256 COMMENT 'ac支持最大主网关个数' ,
`ac_ctrl_port`  int(11) NOT NULL DEFAULT 5246 COMMENT '控制信令收发端口' ,
`ac_data_port`  int(11) NOT NULL DEFAULT 5247 COMMENT '数据信息收发端口' ,
PRIMARY KEY (`ac_mac`)
) COMMENT='AC基本信息表';


CREATE TABLE `ac_controller_network` (
`controller_mac`  char(6) NOT NULL DEFAULT '1A2B3C' COMMENT '主网关MAC地址，16进制表示' ,
`network_number_start`  varchar(46)  NOT NULL DEFAULT '172.31.0.10'  COMMENT '主网关网络号起始地址'  ,
`network_number_end`  varchar(46) NOT NULL DEFAULT '172.31.0.254' COMMENT '主网关网络号终止地址'  ,
`network_ip`  varchar(46) NOT NULL DEFAULT '172.31.0.1' COMMENT '主网关IP地址' ,
`network_subnet_mask`  char(15) NOT NULL DEFAULT '255.255.0.0' COMMENT '主网关子网掩码' ,
`network_primary_dns`  varchar(46)  NOT NULL DEFAULT '172.31.0.1' COMMENT '主网关首选DNS IP地址' ,
`network_secondary_dns`  varchar(46) NOT NULL DEFAULT '172.31.0.1' COMMENT '主网关备选DNS IP地址' ,
`network_status`  tinyint(4) NOT NULL DEFAULT 0 COMMENT '该条记录是否已应用于网关|[0:未应用][1:已应用]' ,
PRIMARY KEY (`controller_mac`)
) COMMENT='主网关网络配置表' ;


CREATE TABLE `ac_controller_radio` (
`index`  int(11) NOT NULL AUTO_INCREMENT COMMENT '记录数' ,
`controller_mac`  char(6)  NOT NULL DEFAULT '1A2B3C' COMMENT '主网关MAC地址，16进制表示' ,
`radio_type`  tinyint(4) NULL DEFAULT 1 COMMENT '主网关热点类型|[0:2.4G][1:5G]' ,
`radio_index`  tinyint(4) NULL DEFAULT 0 COMMENT '指定WLAN类型下SSID的索引，如2.4G下的第0个SSID' ,
`radio_ssid`  varchar(32) NOT NULL DEFAULT 'acmesh' COMMENT 'ap的ssid' ,
`radio_channel`  int(11) NULL DEFAULT NULL  COMMENT 'radio的通道数值' ,
`radio_encrypt_type`  smallint(6) NULL DEFAULT NULL COMMENT 'radio的加密算法类型' ,
`radio_key`  varchar(32) NOT NULL DEFAULT 'acmesh' ,
`radio_status`  tinyint(4) NOT NULL DEFAULT 0 COMMENT '该条记录是否已应用于网关|[0:未应用][1:已应用]' ,
PRIMARY KEY (`index`)
) COMMENT='主网关RADIO信息表';


CREATE TABLE `ac_controller_roam` (
`controller_mac`  char(6) NOT NULL DEFAULT '1A2B3C' COMMENT '主网关MAC地址，16进制表示' ,
`roam_report_interval`  int(11) NULL DEFAULT 30 COMMENT '主网关定时上报时间间隔，时间单位秒' ,
`roam_timeout`  int(11) NULL DEFAULT 300 COMMENT '主网关数据老化时间，时间单位秒' ,
`roam_blacklist_timeout`  int(11) NULL DEFAULT 5 COMMENT '主网关黑名单老化时间，时间单位秒' ,
`roam_monitor_rssi`  smallint(6) NULL DEFAULT 70 COMMENT '热点信号强度阈值，低于该值表示信号较差' ,
`roam_diff_rssi`  smallint(6) NULL DEFAULT 10 COMMENT '漫游决策阈值，与已连接信号的差值，大于该值则漫游' ,
`roam_status`  tinyint(4) NULL DEFAULT 0 COMMENT '该条记录是否已应用于网关|[0:未应用][1:已应用]' ,
PRIMARY KEY (`controller_mac`)
) COMMENT='主网关漫游信息表';



INSERT INTO `ac_controller_radio` (`index`  ,`controller_mac`  ,`radio_type` ,`radio_index`  ,`radio_ssid`  ,`radio_channel`  ,`radio_encrypt_type`   ,`radio_key`  ,`radio_status` ) values (0,"1A2B3C",0,0,"abc",12,10,"accool",0);
INSERT INTO `ac_controller_radio` (`index`  ,`controller_mac`  ,`radio_type` ,`radio_index`  ,`radio_ssid`  ,`radio_channel`  ,`radio_encrypt_type`   ,`radio_key`  ,`radio_status` ) values (0,"1A2B3C",0,1,"def",12,10,"local",1);
INSERT INTO `ac_controller_radio` (`index`  ,`controller_mac`  ,`radio_type` ,`radio_index`  ,`radio_ssid`  ,`radio_channel`  ,`radio_encrypt_type`   ,`radio_key`  ,`radio_status` ) values (0,"2B3C4D",0,2,"hjk",12,10,"besttry",1);
INSERT INTO `ac_controller_radio` (`index`  ,`controller_mac`  ,`radio_type` ,`radio_index`  ,`radio_ssid`  ,`radio_channel`  ,`radio_encrypt_type`   ,`radio_key`  ,`radio_status` ) values (0,"2B3C4D",0,3,"uvw",12,10,"cockcock",0);
