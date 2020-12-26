# -*- coding: utf-8 -*-

# Define your item pipelines here
#
# Don't forget to add your pipeline to the ITEM_PIPELINES setting
# See: https://docs.scrapy.org/en/latest/topics/item-pipeline.html

import json
class MyPipeline(object):
    def __init__(self):
        self.file = open('items_2.json','w',encoding='utf-8')
        #self.file = open('items.jl','w',encoding='utf-8')

    def process_item(self, item, spider):

        #content = str(item) + '\n'
        #self.file.write(content)
        if item['reply']:
            line = json.dumps(dict(item),ensure_ascii=False) + "\n"
            self.file.write(line)
            return item
