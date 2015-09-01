#!/bin/bash

function avg() {
    return $1+$2
}
sum=$((avg 50 20))
echo $sum

