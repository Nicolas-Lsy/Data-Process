# 地震记录合成

程序对指定文本数据进行处理，筛选出符合条件的地震记录。


一、数据准备


原始文件包括井数据（井数据.xlsx）和子波数据（wave.txt）其中井数据分为三列，第一列为深度（DEPTH），第二列为声波时差（AC），第三列为密度（DEN）。


二、时深转换（程序模块一）


1、速度（V）的求取

利用公式（1），求出各个深度点（DEPTH）上的速度（V）.

2、时间（T）的求取

利用公式（2）
求取在50.01-248.81这段深度上各个DEPTH对应的时间点（T）,其中V即为步骤1（速度（V）的求取）中求出的速度（V）。
公式（1）中T1为初始时间（取值1712），对应的DEPTH为50.01；
然后再利用公式（1）求取DEPTH为50.03对应的时间T2，具体公式为T2=T1+2*1000*（50.03-50.01）/V1，
同理DEPTH为50.05对应的时间T3为,T3=T2+2*1000*（50.05-50.03）/V2，以此论推，最后得出DEPTH为248.81对应的TN。
输出计算后的速度（V）和时间（T）。


3、时间(T)的重采样

时间（T）的值都是小数，现在需要对时间（T）进行重新筛选，筛选的原则是按间隔为2进行选取，也就是说时间（T）的值应为1712、1714、1716、1718……。这样进行筛选势必会删去一些值，从表1中可以看出，很难找出正好是1712、1714、1716……等等的点，这就得需要进行近似选取，表格中的时间（T）中谁和1712、1714、1716……最接近，就选谁近似为1712、1714、1716……，就拿1714来举例，DEPTH为55.07对应的T为1713.994，相比较于DEPTH为55.09对应的T为1714.008，更加接近于1714，故取55.07对应的1713.994近似看为1714。最后输出各个整数时间点（T）（1712、1714、1716、1718……）对应的深度DEPTH点，还有速度（V）和密度（DEN），格式如图2所示。



三、合成地震记录（程序模块2）

1、波阻抗的求取

利用公式（3）求出各个时间（T）点上的波阻抗（Z），输出数据。

2、反射系数的求取

利用公式（5）求取各个时间点（T）上的反射系数（Ri）
例如求取R1，其中R1和时间点T1（1712）对应即可，如果按这个公式递推下去的话，应该会比波阻抗（Z）值少一个点，这个无所谓。

3、地震记录S的求取


假设反射系数（R）的点数为4个，子波（b）的点数为2个，利用上述公式进行计算的具体步骤为：
（1）、当n=1，i=1,2时，S(1)=R(0)*b(1)+R(-1)*b(2);（2）、当n=2，i=1,2时，S(2)=R(1)*b(1)+R(0)*b(2)；
（3）当n=3,b=1,2时; S(3)=R(2)*b(1)+R(1)*b(2)；
（4）当n=4,b=1,2时; S(4)=R(3)*b(1)+R(2)*b(2)；
（5）当n=5,b=1,2时; S(5)=R(4)*b(1)+R(3)*b(2)；
（6）当n=6,b=1,2时; S(6)=R(5)*b(1)+R(4)*b(2)；
由于R(0)、R(-1)、R(5)都不存在，取为0，也就是说S(1)值为0，R(5)*b(1)=0。故实际不为0的S值为5个。
说明：由于反射系数（R）和子波（b）褶积之后，点的个数会增加，假设反射系数（R）一共有N个点，子波有L个点，
则褶积完成后会生成合成地震记录点的个数为：M=N+L-1个点。


.
