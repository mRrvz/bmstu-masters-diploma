import matplotlib.pyplot as plt
from matplotlib.ticker import MultipleLocator
import matplotlib.colors as mcolors

x = [1, 2, 4, 8, 16, 32, 64]
y1 = [16.3, 17.7, 18.2 , 19.5, 20.3, 21.6, 23.5]
y2 = [13.2, 14.2, 15.25 , 16.9, 17.5, 20.5, 21.9]

fix, ax = plt.subplots()
norm = mcolors.Normalize(vmin=0, vmax=1)
plt.plot(x, y1, 'r--', label='Аппаратная реализация', color='black')
plt.plot(x, y2, 'b-', label='Виртуализация', color='black')
plt.legend()
ax.xaxis.set_major_locator(MultipleLocator(norm(8)))
ax.set_ylabel("Скорость чтения (Мб/с)")
ax.set_xlabel("Размер TCP буффера (Кб)")

plt.show()
