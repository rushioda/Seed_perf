import os
import sys

# Specify output directory
#output_dir='/eos/user/j/jstupak/data/flat/VSI/'
#merge_dir='/eos/user/j/jstupak/data/flat/VSI_m/'

output_dir='/eos/atlas/atlascerngroupdisk/perf-idtracking/UTrack/TTrees_v0/InvHiggs_samples/'
merge_dir='/eos/atlas/atlascerngroupdisk/perf-idtracking/UTrack/TTrees_v0_m_31319/'

if not os.path.exists(merge_dir):
  os.makedirs(merge_dir)

for sampleDir in os.listdir(output_dir):
    if 'mc16_13TeV' in sampleDir:continue

    if not os.path.exists(merge_dir+'/'+sampleDir):
      os.makedirs(merge_dir+'/'+sampleDir)

    # Loop over all files, merge into a common output file
    list_of_samples = ''
    for fn in os.listdir(output_dir+'/'+sampleDir):
      for s in os.listdir(output_dir+'/'+sampleDir+'/'+fn):
        list_of_samples += output_dir+'/'+sampleDir+'/'+fn+'/'+s+' '

    output_file_name = '%s/merged_%s' % (merge_dir,fn.replace('.PowhegPythia8EvtGen_','pm.PowhegPythia8EvtGen_').replace('_WmH_','__WH_').replace('_WpH_','_WH_'))
    eff_output_name  = output_file_name.replace('merged','processed')
    # Now Hadd them
    cmd = 'hadd -f %s %s' % (output_file_name,list_of_samples);
    os.system(cmd)

    # After Hadd, run them through our DVEffAnalysis script
    ana_cmd = 'root -l -b -q \'DVeffAnalysis.C+(\"%s\",\"%s\")\'' % (output_file_name,eff_output_name)
    os.system(ana_cmd)

