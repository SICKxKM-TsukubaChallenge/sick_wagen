#!/usr/bin/env python3
# -*- coding: utf-8 -*-

# 走行中に５秒おきに座標（正確にはmapからbase_linkのtf）を取得しcsvに保存するスクリプト。頻度はrospy.Rateで変えられます。起動する前に既存のcsvをrm -rfしておく必要がある。


import roslib
# roslib.load_manifest('sick_wagen')
import rospy
import math
import tf
import geometry_msgs.msg
from sensor_msgs.msg import Joy
import turtlesim.srv
import csv

csvfile = "sick10_1015.csv"

def joy_callback(msg):
    if msg.buttons[2]:
        print("[{0},{1},0.0,0.0,0.0,{2},{3}, 0],".format(trans[0], trans[1], 0.0, 0.0, 0.0, rot[2], rot[3]))
        with open(csvfile, 'a') as f:
            writer = csv.writer(f)
            writer.writerow([trans[0], trans[1], 0.0, 0.0, 0.0, rot[2], rot[3], 0])
        save_signal = 0
        former_save =rospy.Time.now()
        print("Set continue pose")
    elif msg.buttons[3]:
        print("[{0},{1},0.0,0.0,0.0,{2},{3}, 1],".format(trans[0], trans[1], 0.0, 0.0, 0.0, rot[2], rot[3]))
        with open(csvfile, 'a') as f:
            writer = csv.writer(f)
            writer.writerow([trans[0], trans[1], 0.0, 0.0, 0.0, rot[2], rot[3], 1])
        save_signal = 0
        former_save =rospy.Time.now()
        print("Set temporary stop pose")
    

if __name__ == '__main__':

    with open(csvfile, 'w') as fc:
        writer = csv.writer(fc)
    fc.close    

    rospy.init_node('waypoint_saver')

    listener = tf.TransformListener()
    rate = rospy.Rate(1)
    rospy.Subscriber("/joy", Joy, joy_callback)
    while not rospy.is_shutdown():
        try:
            (trans,rot) = listener.lookupTransform('/map', '/base_link', rospy.Time(0))
        except (tf.LookupException, tf.ConnectivityException, tf.ExtrapolationException):
            continue
        
    rospy.Subscriber("/joy", Joy, joy_callback)
    rate.sleep()
