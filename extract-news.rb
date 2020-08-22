#!/usr/bin/env ruby

tag_ref = ARGV[0]
if tag_ref
  version =  tag_ref.sub(/\Arefs\/tags\//, "")
else
  version = nil
end
news = File.read(File.join(__dir__, "news.md"))

entries = news.split(/^## /)[1..-1]
if version
  target_entry = entries.find do |entry|
    entry.start_with?("#{version} ")
  end
else
  target_entry = entries.first
end

puts("## #{target_entry}")
