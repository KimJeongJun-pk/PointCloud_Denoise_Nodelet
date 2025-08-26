- voxel.setLeafSize : voxel 한변의 크기 0.015=1.5cm
- mean_k : 최근접 이웃 수
  - 증가 : 안정적이지만 연산량 오래걸림
  - 감소 : 연산은 빠르지만, 노이즈가 아닌 데이터를 노이즈로 오인할 수 있음
- std_dev_mul_thresh : 표준편차 곱 임계치
  - 증가 : 엄격하게 처리, 노이즈가 아닌 데이터도 날아갈 수 있음
  - 감소 : 관대하게 처리, 노이즈가 남을 수 있음

'''
$ roslaunch points_denoise_nodelet noise_removal.launch
'''
