STM32_Embedded

STM32 ile yaptığım, gömülü yazılım tekniklerini öğrendiğim maker projelerimi paylaşıyorum.

#04Repo_SysTick

Bu kod STM32F407'de SysTick ile 1 ms’lik zaman tabanı üzerinde her LED4’ü 500 ms’de bir
toggle eder. Saat olarak 16MHz HSI seçilmiştir.

SysTick nedir?

System Tick Timer; çekirdeğin içerisinde 24-bitlik, geriye doğru sayan, ayarlanması halinde
0’a ulaştığında interrupt üretebilen bir timer.

LAOD nedir?

LOAD, SysTick’in geri sayımın başlangıç değerini tutan ve register.

LOAD değeri nasıl hesaplanır?

Formül= (saat frekansı x istenile süre)-1. Sistem saati olarak ayarlanan HSI veya HSE’dir.

Frekansı 16MHz olan bir saat demek 1 saniyede 16.000.000 vuruş anlamına gelir.
1ms(milisaniye)’de 16.000 vuruş demek. Bu 16 bin vuruşun 1ms’de(milisaniye) bitmesi
isteniyorsa LOAD değeri 15999 olarak ayarlanır. -1 yapılır çünkü LOAD değerinin 0 olması
da sayılır.

Neden SysTick varken, çekirdeğin dışında da timer’lara ihtiyaç var?

Her iki tip timer’ın yaptığı işler birbirinden farklı. SysTick geriye doğru sayar ve kesme üretir.
Tüm ARM’larda ortaktır ve bu da taşınabilirlik sağlar. ARM dışındaki timer’ların PWM üretimi,
dış sinyalin zamanını yakalama (input capture), belirli sayıda eşleşmede çıkış değiştirme
(output compare), enkoder okuma, ADC’yi ya da DMA’yı tetikleme, birden fazla kanal, 16 ya
da 32 bit sayaç, ön bölücü (prescaler) gibi farklı amaçlarla kullanılma yolları vardır. SysTick
bunları yapmaz.

Tek timer ile çalışmak mümkün mü?

Tüm sistem elemanlarını tek bir periyota zorlamak pratik açıdan mümkün değil. Sistemin
farklı birimleri, çalışma esnasında farklı frekanslara ihtiyaç duyabilir.

Neden ondalıklı sayılarla işlem yapmak için FPU donanımına ihtiyaç duyuluyor? 

Çünkü işlemci içerisindeki ALU tam sayılarla çalışır ve donanım olmadan ondalıklı sayılarla çalışmak
cycle sayısı, CPU yükü, kod boyutu vb. açısından daha maliyetlidir. Bu maliyeti azaltmak
amacıyla işlemci içerisinde FPU denen donanımsal bir birim bulunmaktadır.

