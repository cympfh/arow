#!/usr/bin/env ruby

def lpos(x, y, z)
  x + y > 1.0
end

case ARGV[0]

when "linear"
  1000.times do
    x = rand()
    y = rand()
    z = rand()
    puts "#{lpos(x,y,z) ? 1 : -1} #{x} #{y} #{z}"
  end

when "noise"
  1000.times do
    x = rand()
    ex = rand() / 10
    y = rand()
    ey = rand() / 10
    z = rand()
    ez = rand() / 10
    puts "#{lpos(x,y,z) ? 1 : -1} #{x+ex} #{y+ey} #{z+ez}"
  end

when "sparse"
  
  1000.times do
    pos1 = rand() < 0.3
    pos2 = rand() < 0.3
    fs = []
    fs << [1, rand()] if pos1
    fs << [2, rand()] if pos2
    idx = 3
    10.times do
      idx += rand(20)
      fs << [idx, rand()]
    end
    puts "#{(pos1 or pos2) ? 1 : -1} #{fs.map {|p| p[0].to_s+':'+p[1].to_s}.join ' '}"
  end

else
  puts "Usage: ruby gen.rb (linear|noise|sparse)"
end
