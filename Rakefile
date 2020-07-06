require "bundler/gem_tasks"
require "rake/testtask"
require "rake/extensiontask"

Rake::TestTask.new(:test) do |t|
  t.libs << "test"
  t.libs << "lib"
  t.test_files = FileList["test/**/*_test.rb"]
end

gemspec = Gem::Specification::load("revprof.gemspec")
Rake::ExtensionTask.new('revprof', gemspec) do |ext|
  ext.lib_dir = "lib/revprof"
end
task :build => :compile

task :default => :test
