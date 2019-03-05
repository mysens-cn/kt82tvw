# MYNRF24
mysensor nrf24 with battery cr2032/cr2450 

```
1.门磁功耗： 未触发3ua，触发极值15ma,未有未波器，时间未知，按其它资料单次消耗应该小于500ms

2.继电器操作完好，已知问题如下：HA批量操作没有延时，模块为半双工，大量操作会造成网关瞬间发送
无法接收节点发回ACK，导致状态不对，目前我还在集测中;由于使用非原装挪威产芯片，国产或者山寨丢
包率远超原装水平，也不排除这个原因！

3.I2C由于不同的芯片，对电压要求较高，特别是BH1750，另外CR2032极限电流输出在15ma会产生大量压
降导致电池无法完全使用完，200mah可能只能使用掉150？未知，目前靠代码能轻微解决

```
### 目前BH1750,si7021工作1月27日上电，到目前未止，万用表实测2.9V，工作压降到2.5V，门磁电压与上电一致，波动小于0.2v无法上报

## QQ讨论群

![mynrf24-front](https://github.com/huexpub/MYNRF24/blob/master/doc/Mysenso-QQ.png)

PCB尺寸 : 39.12mmX20.57mm 支持沿虚线切割，切割后使用尺寸20.57mmmX20.06mm

## PIC
### Front pic

![mynrf24-front](https://github.com/huexpub/MYNRF24/blob/master/doc/front.png)


### Back pic

![mynrf24-front](https://github.com/huexpub/MYNRF24/blob/master/doc/back.png)

### package 

![package](https://github.com/huexpub/MYNRF24/blob/master/doc/package.jpg)
