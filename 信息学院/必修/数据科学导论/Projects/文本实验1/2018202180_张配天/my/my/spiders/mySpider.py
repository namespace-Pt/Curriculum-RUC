import scrapy
from my.items import MyItem 

class DmozSpider(scrapy.Spider):
    name = 'my'
    #allowed_domains = ['dmoz.org']
    def start_requests(self):
        
        for i in range(0,500,20):
            url = 'https://movie.douban.com/subject/25865470/comments?start='+str(i)+'&limit=20&sort=new_score&status=P'
            yield scrapy.Request(url=url, callback=self.parse)
    def parse(self,response):
        
        for box in response.xpath('//*[@id="comments"]/div[position()<21]'):
            item = MyItem()
            item['author'] = box.xpath('./div[2]/h3/span[2]/a/text()').extract()
            item['reply'] = box.xpath('./div[2]/p/span/text()').extract()
            yield item
        
        
            
